/* Educational implementation of strlen. */
#include <stddef.h>
#include <stdio.h>

static size_t my_strlen(const char *string)
{
    const char *start = string;
    while (*string != '\0') ++string;
    return (size_t)(string - start);
}

int main(void)
{
    const char *message = "low-level programming";
    printf("Length of \"%s\": %zu\n", message, my_strlen(message));
    return 0;
}
