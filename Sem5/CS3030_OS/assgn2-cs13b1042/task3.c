/* C program to delete all subdirectories and files using system calls  */
/* Written by Akilesh B, CS13B1042  */

#include <stdio.h>
#include <unistd.h>
#include <dirent.h>
#include <sys/types.h>

/*  function which deletes all subdirectories and files  */
int deleteDir(const char *name, int depth)
{
	struct dirent *record;
    DIR *dir;

    if (!(dir = opendir(name)))             //base case checking
    {
        printf("error in opening directory \n");
        return -1;
    }

    if (!(record = readdir(dir)))               //base case checking
    {
        printf("error in reading another directory\n");
        return -1;
    }

    do {
    	char path[1048];                   //to store path
    	int length = snprintf(path, sizeof(path)-1, "%s/%s", name, record->d_name);			//path and length of path are initialized. The content is stored in buffer pointed by variable path.
    	path[length] = 0;

        if (record->d_type == DT_DIR) {             //if it is a directory
            
            if (strcmp(record->d_name, "..") == 0 || strcmp(record->d_name, ".") == 0)              // If it is . or if it is .. then continue
                continue;

            deleteDir(path, depth + 1);				//recurse and go inside files present in the folder
            rmdir(path);							//finally remove the folder
        }
        else {
        	unlink(path);                          //if it is a single file
        }
    } while (record = readdir(dir));

  closedir(dir);
  return 0;
}

int main(int argc, char **argv)                     // main function which takes in path of the main directory as argument
{
    if(argc != 2)    
        return -1;
    int freturn;

    freturn = deleteDir(argv[1], 0);						// Takes in the path of directory with subdirectories and files
    if(freturn == 0)
        printf("successfully deleted\n");
    else
        printf("error in deleting\n");
    
    rmdir(argv[1]);                             // Finally remove the main directory as well

    return 0;
}
