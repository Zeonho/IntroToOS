/*
*  ECHOSERV
* 
*  Copyright (C) 1995 by Sun Microsystems, Inc.
*  All rights reserved.
* 
*  This file is a product of SunSoft, Inc. and is provided for
*  unrestricted use provided that this legend is included on all
*  media and as a part of the software program in whole or part.
*  Users may copy, modify or distribute this file at will.
* 
*  THIS FILE IS PROVIDED AS IS WITH NO WARRANTIES OF ANY KIND INCLUDING
*  THE WARRANTIES OF DESIGN, MERCHANTIBILITY AND FITNESS FOR A PARTICULAR
*  PURPOSE, OR ARISING FROM A COURSE OF DEALING, USAGE OR TRADE PRACTICE.
* 
*  This file is provided with no support and without any obligation on the
*  part of SunSoft, Inc. to assist in its use, correction, modification or
*  enhancement.
* 
*  SUNSOFT AND SUN MICROSYSTEMS, INC. SHALL HAVE NO LIABILITY WITH RESPECT
*  TO THE INFRINGEMENT OF COPYRIGHTS, TRADE SECRETS OR ANY PATENTS BY THIS
*  FILE OR ANY PART THEREOF.
* 
*  IN NO EVENT WILL SUNSOFT OR SUN MICROSYSTEMS, INC. BE LIABLE FOR ANY
*  LOST REVENUE OR PROFITS OR OTHER SPECIAL, INDIRECT AND CONSEQUENTIAL
*  DAMAGES, EVEN IF THEY HAVE BEEN ADVISED OF THE POSSIBILITY OF SUCH
*  DAMAGES.
* 
*  SunSoft, Inc.
*  2550 Garcia Avenue
*  Mountain View, California  94043
*/

/*
 * ECHOSERVER - multi-thread echo server.
 *
 */

#pragma ident "@(#) echoserv.c 1.4 95/04/27 Richard.Marejka@Canada.Sun.COM"

#define	_REENTRANT

/* Include Files		*/

#include <pthread.h>
#include <stdio.h>
#include <signal.h>
#include <errno.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <unistd.h>
#include <ctype.h>
#include <arpa/inet.h>
#include <netdb.h>

/* Constants & Macros		*/

#define	QMAX	5
#define loop	while(1)

/* External References		*/

extern	void	*server( void * );
extern	char	*connection_name( int, char *, int (*)( int, struct sockaddr *, int * ) );

/* External Declarations	*/

pthread_mutex_t		msgcnt_lock	= PTHREAD_MUTEX_INITIALIZER;
int			msgcnt		= 1;

/*
 * Main - The Summer Side of Life
 *
 */

main( int argc, char *argv[] ) {
	struct sockaddr_in	saddr;
	struct sockaddr_in	caddr;
	int			addrlen	= sizeof( struct sockaddr_in );
	int			sock;
	int			one	= 1;
	int			conn;
	unsigned short		port;
	pthread_attr_t		attr;

	switch ( argc ) {
	  case 1 :
		port	= ECHO_PORT;
		break;

	  case 2 :
		port	= atoi( argv[1] );
		break;

	  default:
		fprintf( stderr, "usage: server port\n" );
		exit( 1 );
	}

	saddr.sin_family	= AF_INET;
	saddr.sin_addr.s_addr	= htonl( INADDR_ANY );
	saddr.sin_port		= htons( port );

	if ( ( sock = socket( AF_INET, SOCK_STREAM, 0 ) ) == -1 ) {
		perror( "socket" );
		exit( 1 );
	}

	if ( setsockopt( sock, SOL_SOCKET, SO_REUSEADDR, (char *) &one, sizeof( one ) ) == -1 ) {
		perror( "setsockopt(SOL_SOCKET,SO_REUSEADDR)" );
		exit( 1 );
	}

	if ( bind( sock, (struct sockaddr *) &saddr, sizeof( saddr ) ) ) {
		perror( "bind" );
		exit( 1 );
	}

	if ( listen( sock, QMAX ) == -1 ) {
		perror( "listen" );
		exit( 1 );
	}

	pthread_attr_init( &attr );
	pthread_attr_setdetachstate( &attr, PTHREAD_CREATE_DETACHED );

	while ( ( conn = accept( sock, (struct sockaddr *) &caddr, &addrlen ) ) != -1 ) {
		int		n;
		pthread_t	tid;

		n	= pthread_create( &tid, &attr, server, (void *) conn );

		if ( n ) {
			fprintf( stderr, "echoserver:main:pthread_create: %s\n", strerror( n ) );
			exit( 1 );
		}
	}

	return( 0 );
}

/*
 * server - handle one client connection in the server.
 *
 */

	void *
server( void *arg ) {
	int	fd	= (int) arg;
	char	buf[BUFSIZ];
	size_t	len;
	char	servbuf[BUFSIZ], *sp;
	char	clntbuf[BUFSIZ], *cp;

	sp	= connection_name( fd, servbuf, getsockname );
	cp	= connection_name( fd, clntbuf, getpeername );

	for (;;) {
		int	n;
		char	str[BUFSIZ];
		char	*s;

		n	= read( fd, str, BUFSIZ );

		if ( n == -1 ) {
			perror( "server:read" );
			break;
		} else if ( n == 0 )
			break;

		for ( s=str; *s; ++s )
			*s	= toupper( *s );

		pthread_mutex_lock( &msgcnt_lock );
		sprintf( buf, "%3d, {tcp,%s,%s} %3d: %s", (int) pthread_self(), sp, cp, msgcnt++, str );
		pthread_mutex_unlock( &msgcnt_lock );

		len	= strlen( buf ) + 1;

		if ( write( fd, buf, len ) != len ) {
			perror( "write" );
			exit( 1 );
		}
	}

	close( fd );

	return( 0 );
}

/*
 * connection_name - convert a socket descriptor into a "name.port" string.
 *
 */

	char *
connection_name( int fd, char *bufp, int (*f)( int, struct sockaddr *, int * ) ) {
	int			sinlen	= sizeof( struct sockaddr_in );
	struct sockaddr_in	sin;

	if ( (*f)( fd, (struct sockaddr *) &sin, &sinlen ) == -1 ) {
		perror( "connection_name" );
		exit( 1 );
	}

	sprintf( bufp, "%s.%d", inet_ntoa( sin.sin_addr ), sin.sin_port );

	return( bufp );
}
