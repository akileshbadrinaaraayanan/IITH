/* C program to delete all subdirectories and files using system calls  */
/* Written by Akilesh B, CS13B1042  */

#include <stdio.h>
#include <unistd.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <sys/types.h>
 
int main(int argc, char **argv)                     // main function which performs necessary actions
{
    if(argc != 2)    
        return -1;

    int file=0;

    if((file=open(argv[1],O_RDONLY)) < 0)               
    {
        printf("error in opening file \n");
        return -1;
    }    
    int lseeksize = (int) lseek(file,0,SEEK_END);               //lseek size.
    printf("lseek size: \t%d bytes \n", lseeksize); 

    struct stat flstat;                     // using stat function

    if(stat(argv[1],&flstat) < 0) 
    { 
        printf("error \n");  
        return -1;
    }   
    int statsize = (int) flstat.st_size;                        //flstat size.  
    
    printf("stat size:\t%d bytes\n", statsize);
    printf("number of blocks:\t%d\n", (int) flstat.st_blocks);        //number of blocks

        if(lseeksize == statsize)                               //verify if both are same
                printf("lseek size and stat size match, hence verified \n");
        else
                printf("Sizes don't match \n");
    return 0;
}
