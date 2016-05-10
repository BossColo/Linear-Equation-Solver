#include "project.h"

void Delete(int i, int j, equations *eq)
{
    pnode thisnode = eq->Head[1][i-1];
    if (j == -1)
    {
        while(thisnode->col != -1)
            thisnode = thisnode->rowLink;
        thisnode->value = 0.0;
    }
    else
    {
        if (thisnode->col == j)
        {
            eq->Head[1][i-1] = thisnode->rowLink;
        }
        else
        {
            while (thisnode->rowLink && thisnode->rowLink->col < j)
            {
                thisnode = thisnode->rowLink;
            }
            if (thisnode->rowLink && thisnode->rowLink->col == j)
            {
                pnode thatnode = thisnode->rowLink;
                thisnode->rowLink = thatnode->rowLink;
            }
        }
        if(eq->Head[0][j-1] == NULL)
            return;
        thisnode = eq->Head[0][j-1];
        if (thisnode->row == i)
        {
            eq->Head[0][j-1] = thisnode->colLink;
            free(thisnode);
        }
        else
        {
            while (thisnode->colLink && thisnode->colLink->row < i)
            {
                thisnode = thisnode->colLink;
            }
            if (thisnode->colLink && thisnode->colLink->row == i)
            {
                pnode thatnode = thisnode->colLink;
                thisnode->colLink = thatnode->colLink;
                free(thatnode);
            }
        }
    }
}    

void AddRow(int i, double c, int j, equations *eq)
{
    i--;
    j--;
    int n = eq->n;
    if (i > n || j > n)
    {
        printf("Invalid Add");
        exit(EXIT_FAILURE);
    }
    double val;
    double newval;
    pnode rowi = eq->Head[1][i];
    pnode rowj = eq->Head[1][j];
    for (int k = 1; k <= n; k++)
    {
        val = 0;
        newval = 0;
        if (rowi->col == k)
        {
            val = c * rowi->value;
            if (rowi->rowLink)
                rowi = rowi->rowLink;
        }
        if (rowj->col == k)
        {
            newval = rowj->value + val;
            if (isZero(newval))
                Delete(j+1, k, eq);
            else
                rowj->value = newval;
            if (rowj->rowLink)
                rowj = rowj->rowLink;
        }
        else if (!isZero(val))
        {
            insert(j+1, k, val, eq);
        }
    }
    pnode ynode = eq->Head[0][n];
    for (int k = 1; k<=i; k++)
    {
        ynode = ynode->colLink;
    }
    val = c * ynode->value;
    ynode = eq->Head[0][n];
    for (int k = 1; k<=j; k++)
        ynode = ynode->colLink;
    ynode->value = (ynode->value + val);
}

void Subtract(int i, double c, int j, equations *eq)
{
    i--;
    j--;
    int n = eq->n;
    if (i > n || j > n)
    {
        printf("Invalid Subtract");
        exit(EXIT_FAILURE);
    }
    double val;
    double newval;
    pnode rowi = eq->Head[1][i];
    pnode rowj = eq->Head[1][j];
    for (int k = 1; k <= n; k++)
    {
        val = 0;
        newval = 0;
        if (rowi->col == k)
        {
            val = c * rowi->value;
            if (rowi->rowLink)
                rowi = rowi->rowLink;
        }
        if (rowj->col == k)
        {
            newval = rowj->value - val;
            if (isZero(newval))
                Delete(j+1, k, eq);
            else
                rowj->value = newval;
            if (rowj->rowLink)
                rowj = rowj->rowLink;
        }
        else if (!isZero(val))
        {
            insert(j+1, k, -val, eq);
        }
    }
    pnode ynode = eq->Head[0][n];
    for (int k = 1; k<=i; k++)
    {
        ynode = ynode->colLink;
    }
    val = c * ynode->value;
    ynode = eq->Head[0][n];
    for (int k = 1; k<=j; k++)
        ynode = ynode->colLink;
    ynode->value = (ynode->value - val);
}
            
void SwapRows(int i, int j, equations *eq)
{
    int n = eq->n;
    if (i > n || j > n)
    {
        printf("Invalid Swap");
        exit(EXIT_FAILURE);
    }
    i--;
    j--;
    pnode temp = eq->Head[1][i];
    pnode temp2;
    pnode* ptoi;
    pnode* pfromi;
    pnode* ptoj;
    pnode* pfromj;
    eq->Head[1][i] = eq->Head[1][j];
    eq->Head[1][j] = temp;

    for (int k = 0; k <= n; k++)
    {
        temp = eq->Head[0][k];
        temp2 = temp;

        if(i==0 || temp->row == i+1)
        {
            ptoi = &(eq->Head[0][k]);
            pfromi = &(eq->Head[0][k]->colLink);
        }
        else
        {
            while (temp->colLink && temp->colLink->row < i+1)
                temp = temp->colLink;
            
            ptoi = &(temp->colLink);
            pfromi = &(temp->colLink->colLink);
        }

        if(j==0 || temp2->row == j+1)
        {
            ptoj = &(eq->Head[0][k]);
            pfromj = &(eq->Head[0][k]->colLink);
        }
        else
        {
            while (temp2->colLink && temp2->colLink->row < j+1)
                temp2 = temp2->colLink;
        
            ptoj = &(temp2->colLink);
            pfromj = &(temp2->colLink->colLink);
        }

        if (*ptoi && *ptoj && *ptoi == *ptoj && *pfromi == *pfromj)
        {
            continue;
        }

        else if (*ptoi == NULL || (*ptoi)->row != (i+1))
        {
            if (*ptoj && (*ptoj)->row == (j+1))
            {
                temp = *ptoi;
                *ptoi = *ptoj;
                *ptoj = *pfromj;
                *pfromj = temp;
            }
        }

        else if (*ptoj == NULL || (*ptoj)->row != (j+1))
        {
            if (*ptoi && (*ptoi)->row == (i+1))
            {
                temp = *ptoj;
                *ptoj = *ptoi;
                *ptoi = *pfromi;
                *pfromi = temp;
            }
        }
        
        else if (*ptoi && (*ptoi)->row == (i+1))
        {
            if (*ptoj && (*ptoj)->row == (j+1))
            {
                temp = *ptoi;
                *ptoi = *ptoj;
                *ptoj = temp;
                temp = *pfromi;
                *pfromi = *pfromj;
                *pfromj = temp;
            }
        }
    }

    temp = eq->Head[1][i];
    temp->row = i+1;
    while(temp->rowLink)
    {
        temp = temp->rowLink;
        temp->row = i+1;
    }

    temp = eq->Head[1][j];
    temp->row = j+1;
    while(temp->rowLink)
    {
        temp = temp->rowLink;
        temp->row = j+1;
    }
}
