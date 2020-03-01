/************************
*  Projekt c. 2         *
*  Autor: Jan Vavra     *
*  Login: xvavra20      *
************************/

#include <stdio.h>
#include <math.h>
#include <stdlib.h>
#include <string.h>

//flagy a makra
#define NUM_PROBLEM \
{ \
	fprintf(stderr, "Na vstupu jsou spatne zadany ciselne hodnoty\n"); \
	return 1; \
}
#define ARGV_PROBLEM \
{ \
	fprintf(stderr, "Problem se vstupem, zkontrolujte vstupni argumenty\n"); \
	return 1; \
}
#define STEPS_PROBLEM \
{ \
	fprintf(stderr, "Pocet iteraci neni spravny\n"); \
	return 1; \
}
#define EPS 1e-12

//prototypy funkci
double taylor_log(double x, unsigned int n);
double cfrac_log(double x, unsigned int n);
double taylor_pow(double x, double y, unsigned int n);
double taylorcf_pow(double x, double y, unsigned int n);
double my_log(double x);
double my_pow(double x, double y);

//main
int main(int argc, char *argv[])
{
	double value_x;
	double value_y;
	int steps;
	int log_flag = 0;
	int pow_flag = 0;
	int extr_flag = 0;
	char *leftover = NULL;
	//kontrola, ze mame zadane nejake argumenty
	if (argc == 4)
	{
		//kdyz je 1. argv log
		if (!strcmp(argv[1], "--log"))
		{
			log_flag = 1;

			value_x = strtod(argv[2], &leftover);
			if (*leftover != 0)
				NUM_PROBLEM;
			steps = strtol(argv[3], &leftover, 10);
			if (*leftover != 0)
				NUM_PROBLEM;

			//pokud je pocet iteraci zaporny nebo 0
			if (steps <= 0)
				STEPS_PROBLEM;
		}
	
		//kdyz je 1. argv extra(my_log, my_pow)
		else if (!strcmp(argv[1], "--extra"))
		{
			extr_flag = 1;

			value_x = strtod(argv[2], &leftover);
			if (*leftover != 0)
				NUM_PROBLEM;
			steps = strtol(argv[3], &leftover, 10);
			if (*leftover != 0)
				NUM_PROBLEM;
		
		}
		else
			ARGV_PROBLEM;
	}
	//1. argument je pow
	else if (argc == 5)
	{
		if (!strcmp(argv[1], "--pow"))
		{
			pow_flag = 1;

			value_x = strtod(argv[2], &leftover);
			if (*leftover != 0)
				NUM_PROBLEM;
			value_y = strtod(argv[3], &leftover);
			if (*leftover != 0)
				NUM_PROBLEM;
			steps = strtol(argv[4],  &leftover, 10);
			if (*leftover != 0)
				NUM_PROBLEM;

			if (steps <= 0)
				STEPS_PROBLEM;

		}
		else
			ARGV_PROBLEM;
	}
	else
		ARGV_PROBLEM;

	//pokud je volan logaritmus
	if (log_flag)
	{
		double taylor = taylor_log(value_x, steps);
		double cf = cfrac_log(value_x, steps);
		double control = log(value_x);

		printf("       log(%g) = %.12g\n", value_x, control);
		printf("    cf_log(%g) = %.12g\n", value_x, cf);
		printf("taylor_log(%g) = %.12g\n", value_x, taylor);
	}
	//pokud je volan pow
	else if (pow_flag)
	{
		double taylor = taylor_pow(value_x, value_y, steps);
		double cf = taylorcf_pow(value_x, value_y, steps);
		double control = pow(value_x, value_y);

		printf("         pow(%g, %g) = %.12g\n", value_x, value_y, control);
		printf("  taylor_pow(%g, %g) = %.12g\n", value_x, value_y, taylor);
		printf("taylorcf_pow(%g, %g) = %.12g\n", value_x, value_y, cf);
	}
	//pokud jsou volany my_log, my_pow
	else if (extr_flag)
	{
		double contr_log = log(value_x);
		double contr_pow = pow(value_x, value_y);
		double log = my_log(value_x);
		double pow = my_pow(value_x, value_y);

		printf("       log(%g) = %.7e\n", value_x, contr_log);
		printf("    my_log(%g) = %.7e\n\n", value_x, log);

		printf("   pow(%g, %g) = %.7e\n", value_x, value_y, contr_pow);
		printf("my_pow(%g, %g) = %.7e\n", value_x, value_y, pow);
	}

	return 0;
}

/**Taylorova rada pro vypocet ln(x)
* @desc vypocita logaritmus pomoci taylorovy rady 
* @param double x - parametr, ktery logaritmujeme
* @param unsigned int n - pocet iteraci
* @return double - aproximace ln(x)
*/
double taylor_log(double x, unsigned int n)
{
	double result = 0.0;
	double start_x;

	//podminky pro ln
	if (x == 0.0)
		return -INFINITY;
	else if (x < 0.0)
		return NAN;
	else if (isinf(x))
		return INFINITY;
	else if (isnan(x))
		return NAN;
	//zjisteni do jake meze spada x
	if (x > 0.0 && x <= 1.0)
	{
		x = 1.0 - x;
		start_x = x;

		for (unsigned int i = 1; i <= n; i++)
		{
			result -= x / i;
			x *= start_x;
		}
	}
	else if (x > 1.0)
	{
		x = (x - 1.0) / x;
		start_x = x;

		for (unsigned int j = 1; j <= n; j++)
		{
			result += x / j;
			x *= start_x;
		}
	}

	return result;
}


/**Retezove zlomky pro vypocet ln(x)
* @desc vypocita logaritmus pomoci zretezeneho zlomku
* @param double x - parametr, ktery logaritmujeme
* @param unsigned int n - pocet iteraci
* @return double - aproximace ln(x)
*/
double cfrac_log(double x, unsigned int n)
{
	double result = 0.0;
	
	//podminky pro ln
	if (x < 0.0)
		return NAN;
	else if (x == 0.0)
		return -INFINITY;
	else if (isinf(x))
		return INFINITY;

	x = (x - 1) / (x + 1);

	for (unsigned int i = n; i > 1; i--)
	{
		//zavedu si citatele a jmenovatele a pote vypoctu zlomek
		double denomin = 2.0*i - 1.0;
		double numer = (i - 1.0)*(i - 1.0)*(x*x);
		result = numer / (denomin - result);
	}

	return (2 * x) / (1 - result);
}

/**Taylorova rada pro vypocet a^n
* @desc vypocita a^n pri pouziti taylor_log
* @param double x - zaklad
* @param double y - exponent
* @param unsigned int n - pocet iteraci
* @return double - aproximace a^n
*/
double taylor_pow(double x, double y, unsigned int n)
{
	if (x == 1)
		return x;
	if (y == 0)
		return 1;
	
	double result = 1.0;
	double ln = taylor_log(x, n);
	double taylor = 1.0;
	//pokud je ln NAN nelze s nim pocitat
	if (isnan(ln) || x == 0)
		return NAN;
	else if (isinf(y))
	{
		if (x < 1 && x > 0 && y == -INFINITY)
			return INFINITY;
		else if ((y == INFINITY && x < 1) || (y == -INFINITY && x >1))
			return 0;
	}
	for (unsigned int i = 1; i <= n; i++)
	{
		taylor *= (y*ln) / i;
		result += taylor;
	}
	return result;
}

/**Taylorova rada pro vypocet a^n
* @desc vypocita a^n pri pouziti cfrac_log
* @param double x - zaklad
* @param double y - exponent
* @param unsigned int n - pocet iteraci
* @return double - aproximace a^n
*/
double taylorcf_pow(double x, double y, unsigned int n)
{
	if (x == 1)
		return x;
	if (y == 0)
		return 1;
	double result = 1.0;
	double ln = cfrac_log(x, n);
	double taylor = 1.0;
	//pokud je ln NAN nelze s nim pocitat
	if (isnan(ln) || x == 0)
		return NAN;
	else if (isinf(y))
	{
		if (x < 1 && x > 0 && y == -INFINITY)
			return INFINITY;
		else if ((y == INFINITY && x < 1) || (y == -INFINITY && x >1))
			return 0;
	}
	for (unsigned int i = 1; i <= n; i++)
	{
		taylor *= (y*ln) / i;
		result += taylor;
	}
	return result;
}

/**
* @desc vypocita ln(x) s co nejpresnejsi aproximaci pri pouziti nejmensiho poctu iteraci
* @param double x - parametr, ktery logaritmujeme
* @return double - aproximace ln(x)
*/
double my_log(double x)
{
	//podminky pro ln
	if (x == 0.0)
		return -INFINITY;
	else if (x < 0.0)
		return NAN;

	double result_taylor = 0.0;
	double result_cf = 0.0;
	double control_taylor;
	double control_cf;

	for (unsigned int i = 1;; i++)
	{
		control_taylor = result_taylor;
		control_cf = result_cf;

		result_taylor = taylor_log(x, i);
		result_cf = cfrac_log(x, i);

		//pokud uz se hodnota nemeni, dosahli jsme nepresnejsi aproximace
		if (fabs(result_taylor - control_taylor) < EPS)
			return result_taylor;
		else if (fabs(result_cf - control_cf) < EPS)
			return result_cf;
	}
}

/**
* @desc vypocita a^n s co nejpresnejsi aproximaci pri pouziti nejmensiho poctu iteraci
* @param double x - zaklad
* @param double y - exponent
* @return double - aproximace a^n
*/
double my_pow(double x, double y)
{
	if (x == 1 || x == 0)
		return x;
	double result = 1.0;
	double control;
	unsigned int i = 1.0;
	double ln = my_log(x);
	double taylor = 1.0;

	//pokud je 
	if (isnan(ln))
		return NAN;

	do
	{
		control = result;
		taylor *= (y*ln) / i;
		result += taylor;
		i++;
	//pokud uz se hodnota nemeni, dosahli jsme nepresnejsi aproximace
	} while (fabs(result - control) > EPS);

	return result;

}

