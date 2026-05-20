#include <string.h>
#include <stdio.h>

struct patient{
    int id;
    struct patient *left;
    struct patient *right;
    int height;
};