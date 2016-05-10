#include "project.h"

void insert(int i, int j, double c, equations *eq)
{
    pnode x;
    x = eq->Head[1][i-1];
    pnode newptr = (pnode)malloc(sizeof(node));
    newptr->row = i;
    if (j == (eq->n)+1)
        newptr->col = -1;
    else
        newptr->col = j;
    newptr->value = c;
    
    if (x == NULL)
    {
        eq->Head[1][i-1] = newptr;
        newptr->rowLink = NULL;
    }
    else if (j < x->col)
    {
        newptr->rowLink = x;
        eq->Head[1][i-1] = newptr;
    }
    else
    {
        while (x->rowLink && x->rowLink->col < j && x->rowLink->col !=-1)
        {
            x = x->rowLink;
        }
        if (x->rowLink && x->col == j)
        {
            printf("Can't insert, there's something there\n");
            return;
        }
        newptr->rowLink = x->rowLink;
        x->rowLink = newptr;
    }
    x = eq->Head[0][j-1];
    if (x == NULL)
    {
        eq->Head[0][j-1] = newptr;
        newptr->colLink = NULL;
    }
    else if (i < x->row)
    {
        newptr->colLink = x;
        eq->Head[0][j-1] = newptr;
    }
    else
    {
        while (x->colLink != NULL && x->colLink->row < i)
        {
            x = x->colLink;
        }
        newptr->colLink = x->colLink;
        x->colLink = newptr;
    }
}

void output(equations *eq)
{
    int n = eq->n;
    int theCol;
    node toPrint;
    for (int i = 0; i < n; i++)
    {
        if (eq->Head[1][i])
            toPrint = *eq->Head[1][i];
        else
        {
            for (int j = 0; j < n+1; j++)
                printf("0\t");
            printf("\n");
            continue;
        }
        for (int j = 1; j <= n+1; j++)
        {
            if (toPrint.col == -1)
                theCol = n+1;
            else
                theCol = toPrint.col;
            if (theCol == j)
            {
                printf("%.3f\t", toPrint.value);
                if (toPrint.rowLink)
                    toPrint = *(toPrint.rowLink);
            }
            else
                printf("0\t");
        }
        printf("\n");
    }
    printf("\n\n");
}

void initialize(int n, char **S, equations *eq)
{
    eq->n = n;
    eq->Head[0] = (pnode*)calloc(n+1, sizeof(pnode));
    eq->Head[1] = (pnode*)calloc(n, sizeof(pnode));
    for (int i = 0; i < n; i++)
    {
        eq->Head[0][i] = NULL;
        eq->Head[1][i] = NULL;
    }
    eq->Head[1][n-1] = NULL;
    double val;
    int col;
    char *rest;
    for (int i = 1; i <= n; i++)
    {
        char *row = S[i-1];
        rest = row;
        while (*rest != '=')
        {
            row = rest;
            if (*row != '0')
            {
                val = strtof(row, &rest);
                if (val == 0)
                {
                    if (*rest == '-')
                        val = -1;
                    else
                        val = 1;
                }
                while (*rest != 'x')
                    *rest++;
                *rest++;
                col = strtol(rest, &rest, 10);
                insert(i, col, val, eq);
            }
            while(*rest != '+' && *rest != '-' && *rest != '=')
                *rest++;
            if (*rest == '+' || (*(rest+1) == '0' && *rest != '='))
                *rest++;
        }
        *rest++;
        insert(i, n+1, strtof(rest, &rest), eq);
    }
}

void eqfree(equations *eq)
{
    int n = eq->n;
    for (int i = 0; i < n; i++)
    {
        pnode temp = eq->Head[1][i];
        pnode temp2;
        while (temp != NULL)
        {
            temp2 = (*temp).rowLink;
            free(temp);
            temp = temp2;
        }
    }
    free(eq->Head[0]);
    free(eq->Head[1]);
}

bool isZero(float num)
{
    return fabs(num) < 0.0001;
}

void p2Test(equations *eq)
{
    int n = eq->n;
    printf("(1):\n");
    AddRow(n, 2, 1, eq);
    output(eq);
    printf("(2):\n");
    AddRow(3, -3, 2, eq);
    output(eq);
    printf("(3):\n");
    Subtract(1, 1, 2, eq);
    output(eq);
    printf("(4):\n");
    Subtract(n, 3, n-1, eq);
    output(eq);
    printf("(5):\n");
    SwapRows(1, n, eq);
    output(eq);
    printf("(6):\n");
    SwapRows(n-1, 2, eq);
    output(eq);
    printf("(7):\n");
    for (int i = 1; i <= n; i++)
        Delete(i, 1, eq);
    output(eq);
    printf("(8):\n");
    for (int i = 1; i <= n-1; i++)
        Delete(2, i, eq);
    output(eq);
}

void p3Test(int n, char **S, equations *eq)
{
    output(eq);
    Triangulate(eq);
    output(eq);
    float *ans = Solve(eq);
    printf("Solution:\n");
    printf("x1=%.2f", ans[0]);
    for (int x = 1; x<eq->n; x++)
        printf(", x%i=%.2f", x+1, ans[x]);
    printf("\n\n");
    if (IsSolution(ans, n, S))
        printf("The System of Eqations is Solved!\n");
    else
        printf("The System of Equations could not be solved.\n");
    free(ans);
}

int main()
{
    equations eq;
    char *test1[] = {
        "3x1+5x4=8",
        "2x2-7x3=9",
        "5x1-3x2-10x4=-8",
        "6x2-5x4=1"
    };
    char *test2[] = {
        "-3x1+16x3=6",
        "3x1+2x2=-3",
        "-x1-x2-x3=10",
        "x2+2x3+10x4=7",
        "2x5+x6=0",
        "8x4+3x6=-1"
    };
    char *test3[] = {
        "2x1-4x4+5x6=32",
        "3x1+4x3+2x5+x7+8x8=31",
        "x2+2x4+3x7=-7",
        "x1+x4+2x5+2x8=10",
        "-x1-2x2+5x8=8",
        "x2+2x4+3x6=8",
        "x3+2x5+x7+x8=8",
        "x1-3x5+x8=-5"
    };
    int t1n = 4;
    int t2n = 6;
    int t3n = 8;

    initialize(t1n, test1, &eq);
    printf("Test 1, Step1\n");
    output(&eq);
    printf("Test 1, Step2\n");
    p2Test(&eq);
    eqfree(&eq);

    initialize(t2n, test2, &eq);
    printf("\nTest2, Step1\n");
    output(&eq);
    printf("Test 2, Step2\n");
    p2Test(&eq);
    eqfree(&eq);
    
    initialize(t3n, test3, &eq);
    printf("\nTest3, Step1\n");
    output(&eq);
    printf("Test 3, Step2\n");
    p2Test(&eq);
    eqfree(&eq);

    initialize(t1n, test1, &eq);
    printf("\nTest1, Step3\n");
    p3Test(t1n, test1, &eq);
    eqfree(&eq);

    initialize(t2n, test2, &eq);
    printf("\nTest2, Step3\n");
    p3Test(t2n, test2, &eq);
    eqfree(&eq);

    initialize(t3n, test3, &eq);
    printf("\nTest3, Step3\n");
    p3Test(t3n, test3, &eq);
    eqfree(&eq);

    return 0;
}
