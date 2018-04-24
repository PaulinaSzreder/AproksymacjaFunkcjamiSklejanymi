#include <iostream>
#include <math.h>

#define NUMBERS 47
#define SCALE 2

using namespace std;

void crtMiddlePoints(double *distance, double *points)
{
	double tmp = 0;
	int k = 0;
	for (int i = 0; i < NUMBERS - 1; i++)
	{
		tmp = distance[i + 1] - distance[i];
		tmp = tmp / SCALE;

		for (int j = 0; j < SCALE-1; j++)
			points[k++] = distance[i] + tmp*(j + 1);
	}
}

void thomas_method(double *_a, double *_b, double *_c, double *_d, double *_x)
{
	double *_beta = (double*)malloc(sizeof(double)*NUMBERS);
	double *_gamma = (double*)malloc(sizeof(double)*NUMBERS);

	_beta[0] = -(_c[1] / _b[1]);
	_gamma[0] = _d[1] / _b[1];

	for (int i = 1; i < NUMBERS; i++)
	{
		_beta[i] = _c[i] / (_a[i] * _beta[i - 1] + _b[i]);
		_gamma[i] = (_d[i] - _a[i] * _gamma[i - 1]) / (_a[i] * _beta[i - 1] + _b[i]);
	}

	_x[NUMBERS-1] = _gamma[NUMBERS-1];

	for (int i = NUMBERS - 2; i >0; i--)
	{
		_x[i] = _beta[i] * _x[i + 1] + _gamma[i];
	}

}

double spline_interpolacion(double *distance, double *height, double point)
{
	double *_a = (double*)malloc(sizeof(double)*NUMBERS);
	double *_b = (double*)malloc(sizeof(double)*NUMBERS);
	double *_c = (double*)malloc(sizeof(double)*NUMBERS);
	double *_d = (double*)malloc(sizeof(double)*NUMBERS);


	double *_x = (double*)malloc(sizeof(double)*NUMBERS);
	double deltaPrev = 0;
	double deltaNext = 0;
	double delta = 0;
	double result = 0;
	int i = 0;

	for (i = 1; i < NUMBERS-1; i++)
	{
		deltaNext = distance[i + 1] - distance[i];
		deltaPrev = distance[i] - distance[i - 1];
		_a[i] = deltaPrev / deltaNext;
		_b[i] = 2 * (distance[i + 1] - distance[i - 1]) / deltaNext;
		_c[i] = 1;
		_d[i] = 6 * (((height[i + 1] - height[i]) / pow(deltaNext, 2)) - ((height[i] - height[i - 1]) / deltaPrev*deltaNext));
	}


	for ( i = 0; i < NUMBERS; i++)
	{
		if (point >= distance[i] && point <= distance[i + 1])
			break;

	}

	_x[0] = _x[NUMBERS - 1] = 0;

	//thomas_method(_a + 1, _b + 1, _c + 1, _d + 1, _x + 1);
	thomas_method(_a , _b , _c , _d , _x );


	delta = distance[i + 1] - distance[i];
	deltaNext = distance[i + 1] - point;
	deltaPrev = point - distance[i];

	double tmp1 = 0;
	double tmp2 = 0;

	result = (_x[i] / 6.0);
	result = result *pow(deltaNext, 3);
	tmp1 = delta - delta*deltaNext;
	result = result / tmp1;
	tmp1 = (_x[i + 1] / 6.0);
	tmp1 = tmp1*pow(deltaPrev, 3);
	tmp2 = delta*delta - deltaPrev;
	tmp1 = tmp1 / tmp2;
	result = result + tmp1;
	tmp1 = height[i] * deltaNext;
	tmp1 = tmp1 / delta;
	tmp2 = height[i + 1] * deltaPrev;
	tmp2 = tmp2 / delta;
	tmp1 = tmp1 + tmp2;
	result = result + tmp1;

	free(_x);
	free(_a);
	free(_b);
	free(_c);
	free(_d);


	return result;

}

int main()
{
#pragma region ZMIENNE 

	double *distance; // Odleg³oœci
	double *height; // Wysokoœci 
	double *points; // Punkty poœrednie 
	double *results; // Wyniki 

	distance = (double*)malloc(sizeof(double)* NUMBERS);
	height = (double*)malloc(sizeof(double)* NUMBERS);
	points = (double*)malloc(sizeof(double)*NUMBERS*SCALE);
	results = (double*)malloc(sizeof(double)*NUMBERS*SCALE);

	//double distance[] = { 12, 13, 14, 15, 16 };
	//double height[] = { 24, 25, 23, 20, 16 };

	for (int i = 0; i < NUMBERS; i++)
	{
		cin >> distance[i];
		cin >> height[i];
	}

	for (int i = 0; i < NUMBERS*SCALE; i++)
		points[i] = 0;

	//points[0] = distance[0];
	//for (int i = 1; i < NUMBERS*SCALE; i++)
	//	points[i] = points[i - 1] + 0.5 * 2;

	for (int i = 0; i < NUMBERS*SCALE; i++)
		results[i] = 0;


	crtMiddlePoints(distance, points);
	cout.precision(20);

#pragma endregion

	for (int i = 0; i < NUMBERS; i++)
		results[i] = spline_interpolacion(distance, height, points[i]);

	for (int i = 0; i < NUMBERS; i++)
	{
		//cout /*<< points[i] << ' ' */ << results[i] << endl;
		cout << points[i] << endl;
	}



	return 0;
}