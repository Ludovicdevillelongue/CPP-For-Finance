#pragma once

#include <string>
#define _USE_MATH_DEFINES

#include <iostream>
#include <cmath>
#include <math.h>
#include <algorithm>
#include "Payoff_Option.h"

using namespace std;


//probability density function
double norm_pdf(const double x) {
	return (1.0 / (pow(2 * M_PI, 0.5)))*exp(-0.5*x*x);
}

//cumulative density function
double norm_cdf(const double x) {
	double k = 1.0 / (1.0 + 0.2316419*x);
	double k_sum = k * (0.319381530 + k * (-0.356563782 + k * (1.781477937 + k * (-1.821255978 + 1.330274429*k))));

	if (x >= 0.0) {
		return (1.0 - (1.0 / (pow(2 * M_PI, 0.5)))*exp(-0.5*x*x) * k_sum);
	}
	else {
		return 1.0 - norm_cdf(-x);
	}
}

//underlying density
double density(const int j, const double Spot, const double Strike, const double rate, const double Vol, const double Matu) {
	return (log(Spot / Strike) + (rate + (pow(-1, j - 1))*0.5*Vol*Vol)*Matu) / (Vol*(pow(Matu, 0.5)));
}

//greeks formulas
double delta_call(const int j, const double Spot, const double Strike, const double rate, const double Vol, const double Matu) {
	return norm_cdf(density(1, Spot, Strike, rate, Vol, Matu));
}

double gamma_call(const double Spot, const double Strike, const double rate, const double Vol, const double Matu) {
	return norm_pdf(density(1, Spot, Strike, rate, Vol, Matu)) / (Spot*Vol*sqrt(Matu));
}

double vega_call(const double Spot, const double Strike, const double rate, const double Vol, const double Matu) {
	return Spot * norm_pdf(density(1, Spot, Strike, rate, Vol, Matu))*sqrt(Matu);
}

double theta_call(const double Spot, const double Strike, const double rate, const double Vol, const double Matu) {
	return -(Spot*norm_pdf(density(1, Spot, Strike, rate, Vol, Matu))*Vol) / (2 * sqrt(Matu))
		- rate * Strike*exp(-rate * Matu)*norm_cdf(-density(2, Spot, Strike, rate, Vol, Matu));
}

double rho_call(const double Spot, const double Strike, const double rate, const double Vol, const double Matu) {
	return Strike * Matu*exp(-rate * Matu)*norm_cdf(density(2, Spot, Strike, rate, Vol, Matu));
}


double theta_put(const double Spot, const double Strike, const double rate, const double Vol, const double Matu) {
	return -(Spot*norm_pdf(density(1, Spot, Strike, rate, Vol, Matu))*Vol) / (2 * sqrt(Matu))
		+ rate * Strike*exp(-rate * Matu)*norm_cdf(density(2, Spot, Strike, rate, Vol, Matu));
}

double rho_put(const double Spot, const double Strike, const double rate, const double Vol, const double Matu) {
	return -Matu * Strike*exp(-rate * Matu)*norm_cdf(-density(2, Spot, Strike, rate, Vol, Matu));
}
