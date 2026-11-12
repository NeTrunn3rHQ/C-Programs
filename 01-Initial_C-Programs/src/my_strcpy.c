/*
 * Educational implementation of strcpy.
 * The destination must have enough space; source and destination must not overlap.
 */
#include <stdio.h>

static char *my_strcpy(char *dest, const char *src)
{
    char *start = dest;
    while ((*dest++ = *src++) != '\0') {
        /* Copy through the terminating null byte. */
    }
    return start;
}

int main(void)
{
    const char source[] = "low-level C";
    char destination[sizeof(source)];
    printf("Copied: %s\n", my_strcpy(destination, source));
    return 0;
}
