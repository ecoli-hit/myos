#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"
#include <sched.h>


void find_prime(int p[]){
    close(p[1]);//read only
    int buf;
    int n = read(p[0],&buf,sizeof(int));

    /* current prime */
    if (n == 0){
        exit(0);
    }else{
        printf("prime %d \n",buf);
    }


    /* dropout */
    int num[34];
    int cnt = 0;
    int temp = 0;
    while (n != 0)
    {
        n = read(p[0],(void *)&temp,sizeof(int));

        if ((temp % buf) != 0  ){
            num[cnt++] = temp;
        }
    }
    close(p[0]);

    /* next epoch */
    int n_p[2];
    pipe(n_p);

    int pid = fork();

    if (pid != 0 ){
        /* father thread*/
        close(n_p[0]);/*write only*/
        write(n_p[1],(void *)num,cnt * sizeof(int));
        close(n_p[1]);
        wait(0);
        exit(0);
    }else{
        find_prime(n_p);
    }



    
}

void main(int argc, char const *argv[])
{
    int nums[34];
    for(int i=2;i<=35;i++){
        nums[i-2] = i;
    }
    int p[2];
    pipe(p);
    int pid = fork();


    if (pid == 0){
        // printf("in child ");
        find_prime(p);
        
    }else{
        /*father thread */
        // printf("in father ");
        close(p[0]);
        write(p[1],nums,34 * sizeof(int));
        close(p[1]);
        wait(0);
        exit(0);
    }

}
