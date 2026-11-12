/*
 * Educational implementation of memmove.
 * uintptr_t is used to determine copy direction on conventional flat-address
 * platforms. This is an implementation-oriented exercise, not a replacement
 * for the platform C library.
 */
#include <stdint.h>
#include <stdio.h>
#include <stddef.h>

static void *my_memmove(void *dest, const void *src, size_t count)
{
    unsigned char *destination = dest;
    const unsigned char *source = src;

    if (count == 0U || dest == src) return dest;

    if ((uintptr_t)destination < (uintptr_t)source) {
        for (size_t i = 0U; i < count; ++i)
            destination[i] = source[i];
    } else {
        for (size_t i = count; i > 0U; --i)
            destination[i - 1U] = source[i - 1U];
    }
    return dest;
}

int main(void)
{
    char buffer[32] = "overlapping copy";
    my_memmove(buffer + 4, buffer, sizeof("overlapping copy"));
    puts(buffer);
    return 0;
}
