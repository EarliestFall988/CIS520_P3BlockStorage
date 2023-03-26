#include <stdio.h>
#include <stdint.h>
#include "bitmap.h"
#include "block_store.h"
#include <math.h>
#include <unistd.h>
#include <errno.h>

// YOu might find this handy??
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <errno.h>

// include more if you need

// You might find this handy.  I put it around unused parameters, but you should
// remove it before you submit. Just allows things to compile initially.
#define UNUSED(x) (void)(x)

typedef struct block_store
{
    bitmap_t *bitmap;
    uint8_t *data;
} block_store_t;

block_store_t *block_store_create()
{
    block_store_t *bs_pointer = malloc(sizeof(block_store_t)); //calloc(0,sizeof(pow(256,9)));
    return bs_pointer;
}

void block_store_destroy(block_store_t *const bs)
{

    if(bs == NULL)
    {
        return;
    }

    free(bs);
}
size_t block_store_allocate(block_store_t *const bs)
{

    if(bs == NULL)
    {
        return SIZE_MAX;
    }

    // for(int i = 0; i < 256; i++)
    // {
    //     if(bitmap_get(bs->bitmap, i) == 0)
    //     {
    //         bitmap_set(bs->bitmap, i, 1);
    //         return i;
    //     }
    // }

    UNUSED(bs);
    return 0;
}

bool block_store_request(block_store_t *const bs, const size_t block_id)
{
    UNUSED(bs);
    UNUSED(block_id);
    return false;
}

void block_store_release(block_store_t *const bs, const size_t block_id)
{
    UNUSED(bs);
    UNUSED(block_id);
}

size_t block_store_get_used_blocks(const block_store_t *const bs)
{
    UNUSED(bs);
    return 0;
}

size_t block_store_get_free_blocks(const block_store_t *const bs)
{
    UNUSED(bs);
    return 0;
}

size_t block_store_get_total_blocks()
{
    return 255;
}

size_t block_store_read(const block_store_t *const bs, const size_t block_id, void *buffer)
{
    UNUSED(bs);
    UNUSED(block_id);
    UNUSED(buffer);
    return 0;
}

size_t block_store_write(block_store_t *const bs, const size_t block_id, const void *buffer)
{
    UNUSED(bs);
    UNUSED(block_id);
    UNUSED(buffer);
    return 0;
}

block_store_t *block_store_deserialize(const char *const filename)
{
    UNUSED(filename);
    return NULL;
}

size_t block_store_serialize(const block_store_t *const bs, const char *const filename)
{
    UNUSED(bs);
    UNUSED(filename);
    return 0;
}