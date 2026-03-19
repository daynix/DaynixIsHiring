// TASK: implement buffer overflow/underflow protection by changing
//       our_malloc() and our_free() functions only,
//       call alarm() in case of overflow/underflow happened during the life
//       time of the program
//
//

#include <stdlib.h>
#include <stdio.h>
#include <stdint.h>
#include <unistd.h>
#include <string.h>

// Placeholder
#define CANARY  0xDEADBEEF
#define FREE    0xBEEFBABE
#define POISON  0xAA


typedef struct header {
    size_t size;
    uint32_t canary_start;
    uint32_t is_free;
} header_t;

void alarm_f()
{
    printf("ALARM\n");
    exit(100);
}

void *our_malloc(size_t size)
{
    // [header_t | size(body) | canary]
    size_t total = size + sizeof(header_t) + sizeof(uint32_t);
    void *raw = sbrk(total);
    if (raw == (void *)-1) return NULL;

    header_t *hdr = (header_t *)raw;
    hdr->size = size;
    hdr->canary_start = CANARY;
    hdr->is_free = 0;
    char *puser = (char *) raw + sizeof(header_t);
    uint32_t *end_canary = (uint32_t *)(puser + size);
    *end_canary = CANARY;
    printf("ALLOC %zu bytes\n", size);
    return puser;
}

void our_free(void *ptr)
{
    if (!ptr) return;
    char *puser = (char *)ptr;
    header_t *hdr = (header_t *) (puser - sizeof(header_t));

    if (hdr->canary_start != CANARY) alarm_f();
    if (hdr->is_free == FREE) alarm_f();

    uint32_t *end_canary = (uint32_t *) (puser + hdr->size);
    if (*end_canary != CANARY) alarm_f();
    memset(puser, POISON, hdr->size);
    hdr->is_free = FREE;
    printf("FREE 0x%p\n", ptr);
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
