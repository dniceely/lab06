#include <stdio.h>
#include <stdlib.h>
#include "string.h"

int main(){
    string str1 = string_create("avenged");
    string str2 = string_create("sevenfold");

    if (string_less(str1, str2))
    {
        printf("str1 es menor que str2\n");
    }
    else
    {
        printf("str1 es mayor que str2\n");
    }
    
    if (string_eq(str1, str2))
    {
        printf("str1 y str2 son iguales\n");
    }
    else
    {
        printf("str1 y str2 tienen diferente tamaño\n\n");
    }
    
    str1 = string_destroy(str1);
    str2 = string_destroy(str2);

    return 0;
}