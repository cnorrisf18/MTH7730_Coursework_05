#include "EurPut.h"



double EurPut::PriceByBSFormula(double S0,
    double sigma, double r)
{
    return N(-d_minus(S0, sigma, r)) * K * exp(-r * T) - N(-d_plus(S0, sigma, r)) * S0;
}

double EurPut::DeltaByBSFormula(double S0, double sigma, double r)
{
    return N(d_plus(S0, sigma, r)) - 1.0;
}
double EurPut::ThetaByBSFormula(double S0, double sigma, double r)
{
    double first_term = -(S0 * Nprime(d_plus(S0, sigma, r)) * sigma) / (2.0 * sqrt(T));
    double second_term = r * K * exp(-r * T) * N(d_minus(S0, sigma, r));
    return first_term + second_term;
}