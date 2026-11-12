/* Educational implementation of memset. */
#include <stddef.h>
#include <stdio.h>

static void *my_memset(void *memory, int byte_value, size_t count)
{
    unsigned char *bytes = memory;
    for (size_t i = 0U; i < count; ++i)
        bytes[i] = (unsigned char)byte_value;
    return memory;
}

int main(void)
{
    unsigned char buffer[8];
    my_memset(buffer, 0xA5, sizeof(buffer));
    for (size_t i = 0U; i < sizeof(buffer); ++i)
        printf("%02X%s", buffer[i], i + 1U == sizeof(buffer) ? "\n" : " ");
    return 0;
}
