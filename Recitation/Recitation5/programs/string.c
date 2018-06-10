
#include <stdio.h>
#include <string.h>

int main()
{
   char example[100];
   char example2[100];

   //string copy and concatenation
   strcpy (example,"Phone ");
   

   strcat (example,"number ");
   strcat (example,"is ");
   strcat (example,"10 ");
   printf("final string == %s\n\n",example);   

   //string compare
   strcpy (example,"Phone");
   strcpy (example2,"Phone");

   int out=strcmp(example,example2);
   if (out==0){
	printf("Example and Example 2 are equal\n\n");
   }else{
	printf("Example and Example 2 are different\n\n");
   }
   
   //string length
   int size = strlen (example);
   printf("String length == %d\n\n",size);
    
      

   return 0;
}
