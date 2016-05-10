#include "project.h"

void Triangulate(equations *eq)
{
    pnode one;
    pnode two;
    pnode temp;
    int n = eq->n;
    for (int i = 0; i < n-1; i++)
    {
        one = eq->Head[0][i];
        while(one->row <= i)
        {
            if (one->colLink == NULL)
                break;
            one = one->colLink;
        }
        if (one->row != i+1)
            SwapRows(one->row, (i+1), eq);
        if (one->colLink == NULL)
            continue;
        two = one;
        temp = two->colLink;
        while (temp)
        {
            two = temp;
            temp = two->colLink;
            Subtract(one->row, (two->value)/(one->value), two->row, eq);
        }
    }
}

float* Solve(equations *eq)
{
    int n = eq->n;
    float *ans = calloc(n, sizeof(float));
    float temp;
    float y;
    pnode ynode;
    pnode tempn;
    for (int i = n-1; i>=0; i--)
    {
        temp = 0;
        ynode = eq->Head[0][n];
        while (ynode->row != i+1)
            ynode=ynode->colLink;
        y = ynode->value;
        for (int j = n-1; j>i; j--)
        {
            tempn = eq->Head[0][j];
            while (tempn->row < i+1)
                tempn = tempn->colLink;
            if (tempn->row == i+1)
            {
                temp += tempn->value * ans[j];
            }
        }
        y -= temp;
        tempn = eq->Head[0][i];
        while (tempn->row <i+1)
            tempn = tempn->colLink;
        ans[i] = y/tempn->value;
    }
    return ans;
}

bool IsSolution(float *x, int n, char **S)
{
    equations test;
    pnode tosum;
    pnode ynode;
    float y;
    float sum;
    initialize(n, S, &test);
    for (int i=0; i<n; i++)
    {
        sum = 0;
        ynode = test.Head[0][n];
        while (ynode->row != i+1)
            ynode=ynode->colLink;
        y = ynode->value;
        tosum = test.Head[1][i];
        while (tosum->col != -1)
        {
            sum += (tosum->value * x[tosum->col-1]);
            tosum = tosum->rowLink;
        }
        if (!isZero(y - sum))
            return false;
    }
    return true;
}
