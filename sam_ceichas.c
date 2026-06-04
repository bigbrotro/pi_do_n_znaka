#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_DIGHTS 2000
#define BASIS 4
#define BASE 10000ll



typedef struct  
{
	int dights[MAX_DIGHTS];
	int size;
	int isNegative;

}BigInt;


BigInt initBigInt(BigInt *n)
{
	memset(n->dights, 0, sizeof(n -> dights));
	n->size = 0;
	n->isNegative = 0;

}

void genocid(BigInt *n)
{
	while (n->size > 1 && n->dights[n->size - 1] == 0)
	{
		n->size--;
	}
}


int absolutCompare(BigInt a, BigInt b)
{
	if (a.size != b.size) return a.size > b.size ? 1 : -1;

	for (int i = a.size - 1; i >= 0; i--)
	{
		if (a.dights[i] != b.dights[i])
		{
			return (a.dights[i] > b.dights[i]) ? 1 : -1;
		}
	}
}

void printBigInt(BigInt n) {
	if (n.isNegative) {
		putchar('-');
	}
	printf("%lld", n.dights[n.size - 1]);

	char format[20];
	sprintf(format, "%%0%dlld", BASIS);

	for (int i = n.size - 2; i >= 0; i--) {
		printf(format, n.dights[i]);
	}
	printf("\n");
}


void fromString(BigInt* n, const char* s) {
	initBigInt(n);
	if (strlen(s) == 0) return;

	int startIdx = 0;
	if (s[0] == '-') {
		n->isNegative = 1;
		startIdx = 1;
	}
	else if (s[0] == '+') {
		startIdx = 1;
	}

	int len = strlen(s) - startIdx;
	if (len == 0) {
		initBigInt(n);
		return;
	}

	n->size = 0;
	for (int i = strlen(s); i > startIdx; i -= BASIS) {
		if (n->size >= MAX_DIGHTS) {
			fprintf(stderr, "ERROR: MAX_DIGITS overflow!\n");
			exit(1);
		}
		char temp[10] = { 0 };
		int blockStart = (i - BASIS < startIdx) ? startIdx : i - BASIS;
		int blockLength = (i - BASIS < startIdx) ? i - startIdx : BASIS;

		strncpy(temp, s + blockStart, blockLength);
		n->dights[n->size++] = atoll(temp);
	}
	if (n->size == 0) n->size = 1;
	genocid(n);
}


BigInt fromInt(BigInt *n, int chislo)
{
	initBigInt(n);

	if (chislo < 0)
	{
		chislo = -chislo;
		n->isNegative = 1;
	}
	n->size = 0;

	while (chislo > 0)
	{
		n->dights[n->size++] = chislo % BASE;
		chislo = chislo / BASE;
	}
	
	genocid(n);
}


BigInt absoluteAdd(BigInt a, BigInt b)
{
	BigInt res;

	initBigInt(&res);

	long long flag = 0;
	int max_size = a.size >= b.size ? a.size : b.size;

	for (int i = 0; i < max_size || flag; i++)
	{
		long long sum = flag + (i < a.size ? a.dights[i] : 0) + (i < b.size ? b.dights[i] : 0);
		res.dights[i] = sum % BASE;
		
		flag = sum / BASE;
		if (i >= res.size) res.size++;
	}
	
	return res;
}



BigInt absoluteSubstract(BigInt a, BigInt b)
{
	BigInt res; initBigInt(&res);

	long long flag = 0;

	res.size = a.size;
	
	for (int i = 0; i < a.size; i++)
	{
		long long diff = a.dights[i] - flag - (i < b.size ? b.dights[i] : 0);
		if (diff < 0)
		{
			diff += BASE;
			flag = 1;
		}
		else
		{
			flag = 0;
		}
		res.dights[i] = diff;
	}
	genocid(&res);
	return res;
	
}


BigInt add(BigInt a, BigInt b) {
	BigInt res;
	if (a.isNegative == b.isNegative) {

		res = absoluteAdd(a, b);
		res.isNegative = a.isNegative;
	}
	else {
		if (absolutCompare(a, b) >= 0) {
			res = absoluteSubstract(a, b);
			res.isNegative = a.isNegative;
		}
		else {
			res = absoluteSubstract(b, a);
			res.isNegative = b.isNegative;
		}
	}
	genocid(&res);
	return res;
}


BigInt subtract(BigInt a, BigInt b) {
	BigInt res;
	if (a.isNegative != b.isNegative) {
		res = absoluteAdd(a, b);
		res.isNegative = a.isNegative;
	}
	else {

		if (absolutCompare(a, b) >= 0) {
			res = absoluteSubstract(a, b);
			res.isNegative = a.isNegative;
		}
		else {
			res = absoluteSubstract(b, a);
			res.isNegative = !a.isNegative; 
		}
	}
	genocid(&res);
	return res;
}



BigInt multiplyInt(BigInt a, int b)
{
	BigInt res;
	initBigInt(&res);

	if (b == 0 || (a.size == 1 && a.dights[0] == 0)) return res;


	
	long long absB = b < 0 ? -b : b;
	int resultSign = a.isNegative ^ (b < 0);
	
	long long flag = 0;
	for (int i = 0; i < a.size || flag; i++)
	{	
		if (i >= MAX_DIGHTS)
		{
			fprintf(stderr, "ПЕРЕПОЛНЕНИЕ\n");
			exit(1);
		}

		long long sum = flag + (i < a.size ? a.dights[i] * absB : 0);
		res.dights[i] = sum % BASE;
		flag = sum / BASE;

		if (i >= res.size) res.size++;
	}

	res.isNegative = resultSign;

	genocid(&res);
	return res;
}


BigInt multiply(BigInt a, BigInt b)
{
	
	BigInt res;
	initBigInt(&res);

	if ((a.size == 1 && a.dights[0] == 1) || (b.size == 1 && b.dights[0] == 1)) return res;

	for (int i = 0; i < a.size; i++) 
	{
		long long flag = 0;
		for (int j = 0; j < b.size || flag; j++)
		{
			long long iter = res.dights[i + j] + a.dights[i] * (j < b.size ? b.dights[j] : 0) + flag;
			res.dights[i + j] = iter % BASE;
			
			flag = iter / BASE;

			if (i + j >= res.size) res.size = i + j + 1;
		}
	}


	res.isNegative = a.isNegative ^ b.isNegative;
	genocid(&res);
	return res;

}

long long divideInt(BigInt a, long long b, BigInt *res)
{
	if (b == 0)
	{
		fprintf(stderr, "НА НОЛЬ ДЕЛИТЬ НЕЛЬЗЯ\n");
		exit(1);
	}
	initBigInt(res);
	
	long long flag = 0;
	res->size = a.size;

	long long absB = b < 0 ? -b : b;
	int resultSign = a.isNegative ^ (b < 0);
	

	for (int i = a.size - 1; i >= 0; i--)
	{
		flag = flag * BASE + a.dights[i];
		res->dights[i] = flag / absB;
		flag = flag % absB;

	}

	res->isNegative = resultSign;

	genocid(res);
	return flag;

}


BigInt divide(BigInt a, BigInt b)
{
	if (b.size == 1 && b.dights[0] == 0) fprintf(stderr, "НА НОЛЬ ДЕЛИТЬ НЕЛЬЗЯ"); exit(1);


	int resultSign = a.isNegative ^ b.isNegative;


	int originalANeg = a.isNegative;
	int originalBNeg = b.isNegative;
	a.isNegative = 0;
	b.isNegative = 0;



	BigInt quentent, curent;

	initBigInt(&quentent), initBigInt(&curent);


	if (absolutCompare(a, b) < 0) {
		return quentent;
	}


	for (int i = a.size - 1; i = 0; i--)
	{
		if (!(curent.size == 1 && curent.dights[0] == 0))
		{
			for (int j = curent.size; j > 0; j--)
			{
				curent.dights[j] = curent.dights[j - 1];
			}
			curent.size++;
		}
		curent.dights[0] = a.dights[i];
		genocid(&curent);

		long long x = 0;
		long long low = 0;
		long long high = BASE - 1;

		while (low <= high)
		{
			long long mid = low + (high + low) / 2;
			BigInt temp = multiplyInt(b, mid);
			if (absolutCompare(temp, curent) < 0)
			{
				x = mid;
				low = mid + 1;
			}
			else
			{
				high = mid - 1;
			}

		}

		quentent.dights[i] = x;
		BigInt temp = multiplyInt(b, x);
		curent = absoluteSubstract(curent, temp);

	}


	quentent.size = a.size;
	quentent.isNegative = resultSign;

	genocid(&quentent); 

	return quentent;
}

BigInt factorial(int a)
{
	BigInt res; fromInt(&res, 1);

	for(int i = 1; i <= a; i++)
	{
		
		res = multiplyInt(res, i);

	}
	return res;
}


BigInt arctan(int terms, long long x, BigInt *res)
{	
	
	initBigInt(res);

	BigInt tekushii, sledushii;
	
	BigInt edin; initBigInt(&edin);

	edin.dights[MAX_DIGHTS - 2] = 1;
	edin.size = MAX_DIGHTS - 1;



	divideInt(edin, x, &sledushii);


	for (int i = 0; i < terms; i++)
	{
		long long step = (2 * i) + 1;

		divideInt(sledushii, step, &tekushii);

		if (tekushii.size == 1 && tekushii.dights[0] == 0) {
			break;
		}


		if (i % 2 == 0)
		{
			*res = add(*res, tekushii);
		}
		else
		{
			*res = subtract(*res, tekushii);
		}

		BigInt stepen;

		divideInt(sledushii, x, &stepen);
		divideInt(stepen, x, &sledushii);

	}

}


int main()
{

	printf("Enter n digits of pi, the first n digits will be digits of pi\n");

	int znaki;

	scanf("%d", &znaki);
	
	BigInt x_5, x_239, pi, part1, part2; initBigInt(&pi);

	arctan(znaki, 5, &x_5); arctan(znaki, 239, &x_239);

	part1 = multiplyInt(x_5, 16);
	part2 = multiplyInt(x_239, 4);
	pi = subtract(part1, part2);


	printBigInt(pi);
	
}





