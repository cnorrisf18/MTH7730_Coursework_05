#include "EurOption.h"
#include <cmath>
#include <iostream>
using namespace std;

double Pdf_Payoff(double x, double stdev, double mean, double strike, string call_or_put)
{
    //pdf of the function
    if (x == 0)
    {
        return 0;
    }
    double pi = 4.0 * atan(1.0);
    double first_part = 1.0 / (x * stdev * sqrt(2.0 * pi));
    double numer = pow((log(x) - mean), 2.0);
    double denom = 2 * stdev * stdev;
    //cout << numer << "\t" << log(x) << "\t" << log(x) - mean << "\t" << endl;
    //double second_part = exp(-(pow((log(x) - mean), 2.0)) / (2.0 * pow(stdev, 2.0)));
    double second_part = exp(-numer / denom);
    double pdf = first_part * second_part;
    //cout << first_part << "\t" << second_part << "\t" << stdev << "\t" << mean << endl;
    //payoff of the function
    double payoff = 0;
    if (call_or_put == "CALL")
    {
        if (x - strike > 0)
        {
            payoff = x - strike;
        }
    }
    else if (call_or_put == "PUT")
    {
        if (strike - x > 0)
        {
            payoff = strike - x;
        }
    }
    else
    {
        cout << "NEITHER CALL OR PUT; RETURNING 0" << endl;
        return 0;
    }
    //cout << x << "\t" << strike << "\t" << payoff << "\t" << pdf << "\t" << payoff * pdf << endl;
    return payoff * pdf;
}

double Pricing_Integral(double stdev, double mean, string call_or_put, double strike, double a, double b, double N)
{
    double h = (b - a) / N;
    double sum = 0;
    for (double i = 0; i < N+1; ++i)
    {
        double multiplier = 2.0;
        if (i == 0 || i == N)
        {
            multiplier = 1.0;
        }
        sum += multiplier * Pdf_Payoff((a + (i * h)), stdev, mean, strike, call_or_put);
        //cout << sum << endl;
    }
    return (h / 2.0) * sum;
}


double EurOption::N(double x)
{
    double gamma = 0.2316419;     double a1 = 0.319381530;
    double a2 = -0.356563782;   double a3 = 1.781477937;
    double a4 = -1.821255978;   double a5 = 1.330274429;
    double pi = 4.0 * atan(1.0); double k = 1.0 / (1.0 + gamma * x);
    if (x >= 0.0)
    {
        return 1.0 - ((((a5 * k + a4) * k + a3) * k + a2) * k + a1)
            * k * exp(-x * x / 2.0) / sqrt(2.0 * pi);
    }
    else return 1.0 - N(-x);
}

double EurOption::Nprime(double x)
{
    double pi = 4.0 * atan(1.0);
    return exp(-pow(x, 2.0) / 2.0) / sqrt(2.0 * pi);
}

double EurOption::d_plus(double S0, double sigma, double r)
{
    return (log(S0 / K) +
        (r + 0.5 * pow(sigma, 2.0)) * T)
        / (sigma * sqrt(T));
}

double EurOption::d_minus(double S0, double sigma, double r)
{
    return d_plus(S0, sigma, r) - sigma * sqrt(T);
}


double EurOption::VegaByBSFormula(double S0,
    double sigma, double r)
{
    double pi = 4.0 * atan(1.0);
    return S0 * exp(-d_plus(S0, sigma, r) * d_plus(S0, sigma, r) / 2) * sqrt(T)
        / sqrt(2.0 * pi);
}

double EurOption::GammaByBSFormula(double S0, double sigma, double r)
{
    return Nprime(d_plus(S0, sigma, r)) / (S0 * sigma * sqrt(T));
}

void EurOption::PrintValues(double S0, double sigma, double r)
{
    //cout << S0 << " " << sigma << " " << r << " " << T << " " << K << endl;
    cout << "Option price is " << PriceByBSFormula(S0, sigma, r) << endl;
    cout << "Delta is " << DeltaByBSFormula(S0, sigma, r) << endl;
    cout << "Gamma is " << GammaByBSFormula(S0, sigma, r) << endl;
    cout << "Theta is " << ThetaByBSFormula(S0, sigma, r) << endl;
}

double EurOption::PriceByIntegral(double S0, double sigma, double r)
{
    cout << "Calculating Integral" << endl;
    double mean = log(S0) + ((r - (.5 * pow(sigma, 2.0))) * T);
    double stdev = sigma * sqrt(T);
    double solved_integral = Pricing_Integral(stdev, mean, m_call_or_put, K, 0.0, 10000000.0, 1000000.0);
    return solved_integral;
}