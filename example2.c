#include <stdio.h>
#include <string.h>

int main(int argc, char const *argv[])
{
    char uri[] = "www.google.compe";
    char word[] = "\n";
    char *find;

    find = strchr(uri, 'm');

    printf("%s\n", find);

    return 0;
}
