#ifndef PRO_H_
#define PRO_H_

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <math.h>

typedef struct node
{
    int col;
    int row;
    double value;
    struct node* colLink;
    struct node* rowLink;
} node, *pnode;

typedef struct
{
    int n;
    pnode* Head[2];
} equations;

void insert(int i, int j, double c, equations *eq);
void output(equations *eq);
void initialize(int n, char **S, equations *eq);
void eqfree(equations *eq);
bool isZero(float num);
void p2Test(equations *eq);
void p3Test(int n, char **S, equations *eq);

void Delete(int i, int j, equations *eq);
void AddRow(int i, double c, int j, equations *eq);
void Subtract(int i, double c, int j, equations *eq);
void SwapRows(int i, int j, equations *eq);

void Triangulate(equations *eq);
float* Solve(equations *eq);
bool IsSolution(float *x, int n, char **S);
#endif
