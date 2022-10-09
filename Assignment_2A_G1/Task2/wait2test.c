#include "types.h"
#include "user.h"
#include "stat.h"

int main(){
    int retime;
    int rutime;
    int stime;
    retime = 0;
    rutime = 0;
    stime = 0;
    fork();
    int pid = wait2(&retime, &rutime, &stime);
    printf(1,"pid = %d\n", pid);
    printf(1,"retime = %d\n", retime);
    printf(1,"rutime = %d\n", rutime);
    printf(1,"stime = %d\n", stime);
    for(int i = 0; i<250; i++)printf(1,"*");
    exit();
}