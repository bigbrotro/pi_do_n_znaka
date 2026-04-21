#include <stdio.h>
#include <math.h>

//long double fast_pow(long int a, int step)
//{
//    long double res = 1;
//    while (step > 0)
//    {
//        if (step % 2 == 0) 
//        {
//            res *= a;
//        }
//        a *= a;
//        step = step / 2;
//    }
//    return res;
//}


long double drob(long double x)
{
    long double res = x - (long long int)x;
    return res;
}

long double Sj(int j, int d)
{

    long double sum1 = 0, sum2 = 0, sumi = 0;

    for (int k = 0; k <= d; k++)
    {   
        long long int z = (8 * k) + j;
        long long int q = powl(16, d - k);
        sum1 += (long double)((q % z) / z);
        printf("%lld %lld", q, z);

    }

    printf("%f\n", sum1);

    for (int k = d + 1; k <= d + 7; k++)
    {
        long long int z = (8 * k) + j;
        long long int q = powl(16, d - k);
        sum2 += (long double)((q % z) / z);
        printf("%f\n", sum2);
    }



    return sumi;
}



int main()
{
    printf("%Lf", Sj(1, 10000));


    return 0;
}
