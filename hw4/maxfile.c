#include <stdio.h>
#include <stdlib.h>
#include <dirent.h>
#include <sys/stat.h>
#include <string.h>
#include <unistd.h>

long maxfile(const char *dir_path, long *maxWriteSize, char *maxWriteName, long *maxReadSize, char *maxReadName)
{
    DIR *dir = opendir(dir_path);
    if (!dir)
    {
        printf("error opening directory");
        exit(1);
    }

    long dir_size = 0;
    struct dirent *entry;
    struct stat st;
    while ((entry = readdir(dir))) 
    //loop through all files in dir
    {
        if (strcmp((*entry).d_name, ".") == 0 || strcmp((*entry).d_name, "..") == 0)
        {
            continue;
        }

        char file_path[500];
        sprintf(file_path, "%s/%s", dir_path, (*entry).d_name);
        //create file path for access and recursive call

        
        if (stat(file_path, &st) == -1)
        {
            printf("error opening file");
            continue;
        }

        if (S_ISDIR(st.st_mode))
        //if file is a directory perform recursive call
        {
            long subdir_size = maxfile(file_path, maxWriteSize, maxWriteName, maxReadSize, maxReadName);
            printf("%lld\t%s\n", (long long)subdir_size, file_path);
            dir_size += subdir_size;
        }
        else if (S_ISREG(st.st_mode))
        //calculate size of regular files
        {

            if (access(file_path, W_OK) == 0 && st.st_size > *maxWriteSize)
            //writable files that are greater than maxWriteSize
            {
                *maxWriteSize = st.st_size;
                strcpy(maxWriteName, (*entry).d_name);
            }
            else if (st.st_size > *maxReadSize)
            //non writable files that are greater than maxReadSize
            {
                *maxReadSize = st.st_size;
                strcpy(maxReadName, (*entry).d_name);
            }

            //increment total diskUsage with current file's size
            dir_size += st.st_size;
        }
    }

    closedir(dir);
    return dir_size;
}

int main(int argc, char *argv[])
{
    if (argc != 2)
    {
        printf("incorrect number of args");
        return 1;
    }

    //instantiate empty variables to be adjusted with their pointers
    long maxWriteSize = 0;
    char maxWriteName[500] = "";
    long maxReadSize = 0;
    char maxReadName[500] = "";

    long diskUsage = maxfile(argv[1], &maxWriteSize, maxWriteName, &maxReadSize, maxReadName);

    printf("%lld\t%s\n", (long long)diskUsage, argv[1]);
    printf("Largest writable file: %s\n", maxWriteName);
    printf("Size of largest writable file: %lld bytes\n", (long long)maxWriteSize);
    printf("Largest non-writable file: %s\n", maxReadName);
    printf("Size of largest non-writable file: %lld bytes\n", (long long)maxReadSize);

    return 0;
}