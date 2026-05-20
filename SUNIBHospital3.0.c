#include <string.h>
#include <stdio.h>

struct patient{
    int id;
    struct patient *left;
    struct patient *right;
    int height;
};

struct patient* createPatient(int id){
    struct patient* newPatient = (struct patient*)malloc(sizeof(struct patient));
    newPatient->id = id;
    newPatient->left = NULL;
    newPatient->right = NULL;
    newPatient->height = 1;
    return newPatient;
}

