#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"
#include "kernel/param.h"


void main(int argc, char *argv[]){
    char  *para[MAXARG];
    char  buf[32] ; 
    int pp =1;
    

    para[0] = argv[1];
    for (int i = 2 ;i < argc ; i++){
        para[pp] = argv[i];
        pp++;
    }

    // buf[argc-1 ] = 0;
    // exec(argv[1],buf);

    int len ;
    int cnt = 0 ;
    char * p = buf;
    while (strlen(gets(buf,32))!=0)
    {
        len = strlen(buf);
        p = buf;
        cnt = 0;
        for (int i =0 ; i < len; i++){ 
            if (buf[i] == '\n'){
                buf[i] = '\0';
            }

            if (buf[i] == ' '){
                buf[i] = '\0';
                cnt++;
                para[pp] = malloc(cnt * sizeof(char));
                memcpy(para[pp],p,cnt);
                p += cnt;
                cnt = 0;
                pp++;
            }else{
                cnt++;
                if (i +1 >= len){
                    para[pp] = malloc(cnt * sizeof(char));
                    memcpy(para[pp],p,cnt);
                    p += cnt;
                    cnt = 0;
                    pp++;
                }
            }
        }
        para[pp] = 0;


        int pid = fork();
        if (pid == 0){
            exec(argv[1],para);
            exit(0);
        }

        wait(0);

        pp = argc-1;
    }


    wait(0);
    
    // for (int i=0 ; i< pp;i++){
    //     printf(para[i]);
    // }

    exit(0);
    
    

}