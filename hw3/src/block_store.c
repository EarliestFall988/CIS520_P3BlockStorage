#include <stdio.h>
#include <stdint.h>
#include "bitmap.h"
#include "block_store.h"
#include <math.h>
#include <unistd.h>
#include <errno.h>

// You might find this handy??
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <errno.h>

// include more if you need

typedef struct block_store
{
    bitmap_t *bitmap;
    uint8_t *data;
    size_t used;
} block_store_t;

block_store_t *block_store_create()
{
    block_store_t *bs_pointer = malloc(sizeof(block_store_t));
    bs_pointer->bitmap = bitmap_create(BLOCK_STORE_NUM_BLOCKS);

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
    if (bs == NULL)
        return;

    if (block_id <= bitmap_get_bits(bs->bitmap))
    {
        bitmap_reset(bs->bitmap, block_id);
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
        return bitmap_total_set(bs->bitmap) - 1;
    }

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

// get total number of blocks
size_t block_store_get_total_blocks()
{
    return BLOCK_STORE_AVAIL_BLOCKS; // return total number of blocks
}

// read from the block store
size_t block_store_read(const block_store_t *const bs, const size_t block_id, void *buffer)
{
    if (bs == NULL || buffer == NULL) // check if bs is null or buffer is null
    {
        return 0;
    }
    if (block_id >= BLOCK_STORE_NUM_BLOCKS) // check if block_id is greater than BLOCK_STORE_NUM_BLOCKS
    {
        return 0;
    }

    memcpy(buffer, (void *)&(bs->data)[block_id], BLOCK_SIZE_BYTES); // copy block store to buffer

    return BLOCK_SIZE_BYTES; // return number of bytes read which is BLOCK_SIZE_BYTES
}

// write to the block store
size_t block_store_write(block_store_t *const bs, const size_t block_id, const void *buffer)
{
    // check if bs is null or buffer is null or block_id is greater than BLOCK_STORE_NUM_BLOCKS
    if (bs == NULL || buffer == NULL || block_id >= BLOCK_STORE_NUM_BLOCKS)
    {
        return 0;
    }

    if (bs->data == NULL) // check if data is null
    {
        return 0;
    }

    size_t someResult = block_store_allocate(bs); // allocate block

    if (someResult == SIZE_MAX) // check if block store is full
    {
        return 0;
    }

    bs->data = calloc(1, BLOCK_SIZE_BYTES * BLOCK_STORE_NUM_BLOCKS); // allocate memory for data

    memcpy((void *)&(bs->data)[block_id], buffer, BLOCK_SIZE_BYTES); // copy buffer to block store

    return BLOCK_SIZE_BYTES; // return number of bytes written which is BLOCK_SIZE_BYTES
}

// deserialize the block store from the file
block_store_t *block_store_deserialize(const char *const filename)
{

    if (filename == NULL) // check if filename is null
    {
        return NULL;
    }

    int f = open(filename, O_RDONLY); // open file with read only permissions

    if (f == -1) // check if file is null
    {
        return NULL;
    }

    block_store_t *bs = block_store_create(); // create block store

    ssize_t result = read(f, bs, BLOCK_STORE_NUM_BLOCKS * BLOCK_SIZE_BYTES); // read from file

    close(f); // close file

    if (result == -1) // check if result is null
    {
        return NULL;
    }

    return bs; // return block store
}

// serialize the block store to the file
size_t block_store_serialize(const block_store_t *const bs, const char *const filename)
{

    if (bs == NULL || filename == NULL) // check if bs is null or filename is null
    {
        return 0;
    }

    int file = open(filename, O_WRONLY | O_CREAT | O_TRUNC, 0666); // open file

    if (file == -1) // check if file is null
    {
        return 0;
    }

    size_t result = write(file, bs, BLOCK_STORE_NUM_BLOCKS * BLOCK_SIZE_BYTES); // write to file

    close(file); // close file

    return result;
}