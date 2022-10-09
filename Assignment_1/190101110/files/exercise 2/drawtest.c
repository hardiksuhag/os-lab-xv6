#include "types.h"
#include "stat.h"
#include "user.h"

//set maximum buffer size
const int max_size = 6000;

int main(void)
{

    // allocate the size to buffer pointer 
    void *buffer = malloc(max_size);

    // system call with image size as return value
    int picsize = draw(buffer, max_size);

    // Call returns -1 if size of image is greater than max_size
    if (picsize == -1)
    {
        // print message to stdout
        printf(1, "Insufficient buffer size\n");
        exit();
    }
    
    // print the image in buffer to stdout
    printf(1, "%s\n", (char *)buffer);
    exit();
}