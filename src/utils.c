#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include "chat1002.h"

void strtoupper(char * string){
    for (int i = 0; i < strlen(string); i++) 
        string[i] = toupper(string[i]);
}