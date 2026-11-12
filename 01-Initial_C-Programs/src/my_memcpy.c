/*
 * Educational implementation of memcpy.
 * Like the C standard function, source and destination must not overlap.
 */
#include <stddef.h>
#include <stdio.h>

static void *my_memcpy(void *dest, const void *src, size_t count)
{
    unsigned char *destination = dest;
    const unsigned char *source = src;
    for (size_t i = 0U; i < count; ++i)
        destination[i] = source[i];
    return dest;
}

int main(void)
{
    const char source[] = "systems";
    char destination[sizeof(source)];
    my_memcpy(destination, source, sizeof(source));
    printf("Copied string: %s\n", destination);
    return 0;
}
