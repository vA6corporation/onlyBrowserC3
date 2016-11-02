#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int main(void)
{
    FILE * fp;
    char * line = NULL;
    size_t len = 0;
    ssize_t read;
    char uri[] = "https://www.facebook.com/MuniSatipoo/";
    char frase[] = "hola";

    fp = fopen("program.txt", "r");
    if (fp == NULL)
        return 0;



    while ((read = getline(&line, &len, fp)) != -1)
    {
        printf("%s", line);
    }

    fclose(fp);
    return 0;
}
