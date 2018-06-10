#CSCI 4061 Recitation 1





####chmod 700 testfile


u   g   o
--- --- ---
 7   0   0
111 000 000



####chmod g+wr testfile
u   g   o
--- --- ---
	wr


####head -# give the number of first # line you want to see
####tail -#


	chen4566@csel-kh1250-15:~$ x=5
	chen4566@csel-kh1250-15:~$ echo $x
	5
	chen4566@csel-kh1250-15:~$ bash
	chen4566@csel-kh1250-15:~$ echo $x

	chen4566@csel-kh1250-15:~$ exit
	exit
	chen4566@csel-kh1250-15:~$ echo $x
	5
	chen4566@csel-kh1250-15:~$ export y = 10
	bash: export: `=': not a valid identifier
	bash: export: `10': not a valid identifier
	chen4566@csel-kh1250-15:~$ export y=10
	chen4566@csel-kh1250-15:~$ echo $y
	10
	chen4566@csel-kh1250-15:~$ bash 
	chen4566@csel-kh1250-15:~$ echo $x

	chen4566@csel-kh1250-15:~$ echo $y
	10
	chen4566@csel-kh1250-15:~$ 


####Set global varible
	export variableName value

find / -name "filename"



alias rm ="rm -i"

