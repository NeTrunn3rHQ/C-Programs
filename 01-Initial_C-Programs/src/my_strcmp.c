/* Educational implementation of strcmp. */
#include <stdio.h>

static int my_strcmp(const char *left, const char *right)
{
    while (*left != '\0' && *left == *right) {
        ++left;
        ++right;
    }
    return (unsigned char)*left - (unsigned char)*right;
}

int main(void)
{
    const char *first = "memory";
    const char *second = "memory";
    int result = my_strcmp(first, second);
    puts(result == 0 ? "Strings are equal." :
         result < 0 ? "First string is less." : "First string is greater.");
    return 0;
}
