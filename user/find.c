#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"
#include <sched.h>
#include "kernel/fs.h"


char*
fmtname(char *path)
{
  /*获取路径中的文件名*/
  static char buf[DIRSIZ+1];
  char *p;

  // Find first character after last slash.
  for(p=path+strlen(path); p >= path && *p != '/'; p--)
    ;
  p++;

  // Return blank-padded name.
  if(strlen(p) >= DIRSIZ)
    return p;
  memmove(buf, p, strlen(p));
  buf[strlen(p)] = '\0';
  return buf;
}

void find(char path[], char file[]){
    int fd ;
    char buf[512], *p;
    struct dirent de;
    struct stat st;

    // printf(" in find \n");
    if((fd = open(path, 0)) < 0){
        fprintf(2, "ls: cannot open %s\n", path);
        return;
    }

    if(fstat(fd, &st) < 0){
        fprintf(2, "ls: cannot stat %s\n", path);
        close(fd);
        return;
    }
    
    // printf("correct opened and right stat\n");
    // printf("path = %s  %d \n",path,st.type);
    switch(st.type){
      case T_FILE:

        // printf(fmtname(path));
        if (!strcmp(fmtname(path),file)){
            printf("%s\n",path);
        }
        break;

      case T_DIR:
        if(strlen(path) + 1 + DIRSIZ + 1 > sizeof buf){
            printf("ls: path too long\n");
            break;
        } 

        strcpy(buf,path);

        p = buf + strlen(path);
        *p++ = '/';
        // printf(p);
        while (read(fd,&de,sizeof(de)) == sizeof(de)){
            if(de.inum ==0 || de.inum == 1 || !strcmp(de.name,".")|| !strcmp(de.name,"..")){\
                // printf("continue \n");
                continue;
            }
            memmove(p,de.name,strlen(de.name));
            p[strlen(de.name)] = '\0';
            find(buf,file);
        }

        break;
    }
  close(fd);
}

void main(int argc, char *argv[])
{
    if (argc < 3){
        printf("wrong args , need specific path and filename ! \n");
        exit(0);
    }

    find(argv[1], argv[2]);
    exit(0);
}
