// Copyright (c) Microsoft Corporation. All rights reserved.
// Licensed under the MIT License.

#define _GNU_SOURCE
#include <assert.h>
#include <stdlib.h>
#include <string.h>
#include <sys/mman.h>
#include <unistd.h>
#include <openenclave/internal/malloc.h>

#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wmissing-prototypes"

void* __libunwind_mmap(
    void* addr,
    size_t length,
    int prot,
    int flags,
    int fd,
    off_t offset)
{
    void* result = MAP_FAILED;

    if (addr || fd != -1 || offset)
        goto done;

    if (prot != (PROT_READ | PROT_WRITE))
        goto done;

    if (flags != (MAP_PRIVATE | MAP_ANONYMOUS))
        goto done;

    result = __oe_memalign(4096, length);

done:

    return result;
}

int __libunwind_munmap(void* addr, size_t length)
{
    if (!addr)
        return -1;

    if (length)
        __oe_free(addr);

    return 0;
}

int __libunwind_msync(void* addr, size_t length, int flags)
{
    return 0;
}

int __libunwind_mincore(void* addr, size_t length, unsigned char* vec)
{
    if (!addr || !vec)
        return -1;

    size_t n = (length + getpagesize() - 1) / getpagesize();
    memset(vec, 1, n);

    return 0;
}
