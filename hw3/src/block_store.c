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
    size_t used;
} block_store_t;

block_store_t *block_store_create()
{
    block_store_t *bs_pointer = malloc(sizeof(block_store_t)); // calloc(0,sizeof(pow(256,9)));
    bs_pointer->bitmap = bitmap_create(BLOCK_STORE_NUM_BLOCKS);
    // need to put bitmap into data
    bitmap_set(bs_pointer->bitmap, 127);
    return bs_pointer;
}

void block_store_destroy(block_store_t *const bs)
{

    if (bs == NULL)
    {
        return;
    }

    free(bs);
}
size_t block_store_allocate(block_store_t *const bs)
{

    if (bs == NULL)
    {
        return SIZE_MAX;
    }

    size_t first_zero = bitmap_ffz(bs->bitmap);

    if (first_zero > BLOCK_STORE_AVAIL_BLOCKS)
    {
        return SIZE_MAX;
    }

    bitmap_set(bs->bitmap, first_zero);
    bs->used++;

    if (first_zero >= 128)
        first_zero--;

    return first_zero;
}

bool block_store_request(block_store_t *const bs, const size_t block_id)
{
    if (bs == NULL)
    {
        return false;
    }
    if (block_id >= bitmap_get_bits(bs->bitmap))
    {
        return false;
    }
    if (!(bitmap_test(bs->bitmap, block_id)))
    {
        bitmap_set(bs->bitmap, block_id);
        return true;
    }
    return false;
}

void block_store_release(block_store_t *const bs, const size_t block_id)
{
    if (bs != NULL)
    {
        if (block_id >= bitmap_get_bits(bs->bitmap))
        {
            bitmap_reset(bs->bitmap, block_id);
        }
    }
}

size_t block_store_get_used_blocks(const block_store_t *const bs)
{
    if (bs == NULL)
    {
        return SIZE_MAX;
    }
    else
    {
        // return bs->used;
        return bitmap_total_set(bs->bitmap) - 1;
    }
    /* for(int i = 0; i < 256; i++)     //iterates through blocks
    {
        if(bitmap_test(bs->bitmap,i))
        {
            count++;                    //add to counter if set
        }

    } */

    // UNUSED(bs);
    // return count;                       //return count
}

size_t block_store_get_free_blocks(const block_store_t *const bs)
{
    if (bs == NULL)
    {
        return SIZE_MAX;
    }
    return BLOCK_STORE_NUM_BLOCKS - bitmap_total_set(bs->bitmap);
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