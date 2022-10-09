#include "types.h"
#include "x86.h"
#include "defs.h"
#include "date.h"
#include "param.h"
#include "memlayout.h"
#include "mmu.h"
#include "proc.h"

int
sys_fork(void)
{
  return fork();
}

int
sys_exit(void)
{
  exit();
  return 0;  // not reached
}

int
sys_wait(void)
{
  return wait();
}

int
sys_kill(void)
{
  int pid;

  if(argint(0, &pid) < 0)
    return -1;
  return kill(pid);
}

int
sys_getpid(void)
{
  return myproc()->pid;
}

int
sys_sbrk(void)
{
  int addr;
  int n;

  if(argint(0, &n) < 0)
    return -1;
  addr = myproc()->sz;
  if(growproc(n) < 0)
    return -1;
  return addr;
}

int
sys_sleep(void)
{
  int n;
  uint ticks0;

  if(argint(0, &n) < 0)
    return -1;
  acquire(&tickslock);
  ticks0 = ticks;
  while(ticks - ticks0 < n){
    if(myproc()->killed){
      release(&tickslock);
      return -1;
    }
    sleep(&ticks, &tickslock);
  }
  release(&tickslock);
  return 0;
}

// return how many clock tick interrupts have occurred
// since start.
int
sys_uptime(void)
{
  uint xticks;

  acquire(&tickslock);
  xticks = ticks;
  release(&tickslock);
  return xticks;
}

// fills the buffer with ascii image art for sufficient buffer 
// size, else returns -1
int sys_draw(void)
{

  char *picture =                                                                                                         
  "                                        ^^.--::::::::--.^^\n\
                                  ^-:+oyhhddddddddddddddddhhyo+:-^\n\
                             ^-/oyhdddddddso++//:::://++ossyhddddhyo/-^\n\
                          ./shdddddddddddhsoo++/:-.       ^^^.:/oyhdddhs/.\n\
                       ./yhdddddddddddddhhhhddddddhyo/.          ^.-+shddhy/.\n\
                    ^:shdddddddddddhyo/-......-:/oyhddhy/.^^^^^^.^^^^^.yddddhs:^\n\
                  .+hddddddddddhs+-.               .:ohddhyyyhhhhhhyyyshddsohddy+.\n\
                .ohdddddddddho:.                      ^+ddddddddddddddddddy-./yddh+.\n\
              ^+hdddddddddy/^                          -dddddddddddddddddddd+^^:yddh+^\n\
             :hddh++//yddh^                            -dddddddddddddddddddddy.  :hddy:\n\
           ^oddd+^    hddo                             :ddddddddddddddddddddddh-  ^+dddo^\n\
          .hddy-     ^ddd-                             :dddddddddddddddddddddddd:   -yddy.\n\
         :dddo^      -ddd^                             +ddddddddddddddddddddddddd:   ^oddd:\n\
        /ddd/        :ddd^                             odddddddddddddddddddddddddd:    /ddd:\n\
       :ddd/         -ddd^                        ^-/shdddddddddddddddddddddddddddds:^  /ddd:\n\
      -ddd/          .ddd-                     -/shdddhsooydddddddddddddddddddddddhddho-^/ddd-\n\
     ^hddo           ^ddds/:-^             ^:ohdddhs/-^   ^./shdddddddddddddddddy-^:sdddy/sddh^\n\
     oddy^          :ydddddddhyo/-.     ./shddds+-^           ^-oyddddddddddddy:^    .+hddddddo\n\
    -ddd-         .sddddddddddddddhyo::shddhs/.^                 ^.+yddddddhs-^        ^/yddddd-\n\
    sddy         :hdddddddddddddddddddddhs:.                        ^.+ddd+.             ^/hddds\n\
   ^ddd:        +ddddddddddddddddddddddh.                             ^ddd:                .yddd^\n\
   /ddd^       odddddddddddddddddddddddh                               hdd/                 ^ddd/\n\
   oddy       +ddddddddddddddddddddddddd                               ydd+                  yddo\n\
   yddo^^^^^^:dddddddddddddddddddddddddd                               sddo                  oddy\n\
   hddhyyyyyyddddddddddddddddddddddddddd^                              odds                  +ddh\n\
   hdddhhhhhhhdddddddddddddddddddddddddd^                              oddy                  +ddh\n\
   yddo      .hddddddddddddddddddddddddd^                              +ddy                  oddy\n\
   oddy       -ddddddddddddddddddddddddd^                              +ddy                  yddo\n\
   :ddd^       -hddddddddddddddddddddddd                               +ddy                 ^ddd:\n\
   ^ddd/        .ydddddddddddddddddddddd/.                             +ddy                ^oddd^\n\
    oddy         ^sddddddddddddddddddyhddhs:^                       ^:oddddo.            ^/hdddo\n\
    .ddd:          /dddddddddddhyo/-^^^:shddhs:.                 ^:ohdddddddhs-        ^/yddddd.\n\
     +ddh^          -ddddyso/-.^^        .:shddhy+-^          .:ohddddddddddddds-    ./ydddddd+\n\
     ^ydds           ddd+^                  .:oydddhs/-^  ^-+shddddddddddddddddddo.-ohddy+hddy^\n\
      .hdd+          ddd:                      ^-/shdddysshdddddddddddddddddddddddhddhs:.oddh.\n\
       -hdd+         ddd:                          ^-+sdddddddddddddddddddddddddddds/.  +ddh-\n\
        -hddo^       ddd/                              sdddddddddddddddddddddddddd/   ^oddh-\n\
         -hdds.      hdd+                              +ddddddddddddddddddddddddd/   .sddh-\n\
          .yddh:     sdds                              +dddddddddddddddddddddddd:   :hdds.\n\
            +dddo^   /ddd^                             /ddddddddddddddddddddddh-  .sddd+\n\
             .ydddhyyyddd/                             /dddddddddddddddddddddo^ ^+dddy.\n\
               :hdddddddddy/^                          +dddddddddddddddddddh- ^+hddh:\n\
                ^/hdddddddddds/.                    ^-ohddddddddddddddddddy^-odddh/^\n\
                  ^:ydddddddddddy+:^            .-/shdddy+++ooooooo++/:yddhydddy:^\n\
                    ^-ohdddddddddddhyo/---::+oshhdddhs/.^            ^-yddddho-\n\
                       ^:ohdddddddddddddddddddhyso/.^            ^-/ohdddho:^\n\
                          ^-+shddddddddddh+/:..^          ^.-:/oyhdddhs+-^\n\
                              .-/syhddddddyysoo++++++oosyyhhddddhys/-.\n\
                                ^.-:+osyyhhhddddddddhhhyyso+:-.^\n\
                                          ^^...------...^^\n\n";

  char *buffer; // to store the ACII image art
  int max_size; // to store the max buffer size
  int picsize = 0; // to store the size of image

  //Calculating size of the image
  while (picture[picsize] != '\0')
  {
    picsize++;
  }

  // Get the max size of buffer from call argument (at index 1)
  // Return -1 if invalid address is accessed
  if (argint(1, &max_size) == -1)
  {
    return -1;
  }

  // Get buffer pointer from call argument and set alot buffer memory size 
  // equal to max_size
  // Return -1 if invalid address is accessed
  if (argptr(0, (char **)&buffer, max_size) == -1)
  {
    return -1;
  }

  // return -1 if the size of image is greater than max size
  if (picsize > max_size)
  {
    return -1;
  }

  //copy the image to the buffer
  for (int i = 0; i < picsize; i++)
  {
    buffer[i] = picture[i];
  }
  //return the size of the image
  return picsize;
}