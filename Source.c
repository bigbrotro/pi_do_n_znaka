#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <math.h>
#include <locale.h>





long long int fast_pow(long long int step, int k)
{   

    long long int res = 1;
    long long int base = 16 % k;

    
    while (step > 0)
    {   
        if (step % 2 == 1)
        {
            res = (res * base) % k;
        }
        base = (base * base) % k;
        
        step /= 2;

    }
    return res % k;
}



long double Sj(int j, int d)
{

    double s = 0.0;

    for (int k = 0; k <= d; k++)
    {
        long long m = 8 * k + j;
        
        double iter = (double)fast_pow(d - k, m) / m;

        s += iter - (int)iter;
    }
    for (int k = d + 1; k <= d + 20; k++)
    {
  
        double iter = pow(16.0, (double)d - k) / (8 * k + j);
        s += iter;
    }

    return s - (int)s;
}



int main()
{   
    setlocale(LC_ALL, "Rus");

    int n;

    printf("¬ведите какое число хотите увидеть\n");

    scanf("%d", &n);

    double x = 4 * Sj(1, n) - (2 * Sj(2, n)) - (Sj(5, n)) - (Sj(6, n));

    x = x - (int)x;
    if (x < 0)
    {
        x += 1;
    }

    for (int i = 0; i < 8; i++)
    {
        x *= 16;
        int dight = (int)x;
        printf("%X", dight);
        x -= dight;
    }
    printf("\n");

    return 0;
}