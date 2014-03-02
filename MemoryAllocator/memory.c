// TASK: implement buffer overflow/underflow protection by changing
//       our_malloc() and our_free() functions only,
//       call alarm() in case of overflow/underflow happened during the life
//       time of the program
//
// TIP: go to http://www.compileonline.com/compile_c_online.php
//      for an online IDE
//
// Send code to jobs@daynix.com
//

#include <malloc.h>
#include <stdlib.h>

void alarm()
{
    printf("ALARM\n");
    exit(100);
}

void *our_malloc(size_t size)
{
    printf("ALLOC %d bytes\n", size);
    return malloc(size);
}

void our_free(void *ptr)
{
    printf("FREE 0x%p\n", ptr);
    free(ptr);
}

#define malloc our_malloc
#define free our_free

int main()
{
    char *x = malloc(64);

    if(!x)
    {
        printf("ALLOC FAILED\n");
        return -1;
    }

    //Buffer underflow
    x[-1] = 0;

    //Buffer overflow
    x[64] = 0;

    free(x);
    return 0;
}

