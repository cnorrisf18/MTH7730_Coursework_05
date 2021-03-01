#include "UpAndOutEuropeanCall.h"
#include <cmath>

#include <vector>
using namespace std;
double CumulativeIntegral(double x)
{
    return exp(-pow(x, 2) / 2);
}
double Pricing_Integral(double a, double b, double N)
{
    double h = (b - a) / N;
    double sum = 0;
    for (double i = 0; i < N + 1; ++i)
    {
        double multiplier = 2.0;
        if (i == 0 || i == N)
        {
            multiplier = 1.0;
        }
        sum += multiplier * CumulativeIntegral((a + (i * h)));
    }
    return (h / 2.0) * sum;
}
double N(double z) { //Cumulative Normal Distribution function

    const double pi = 4.0 * atan(1.0);
    double integral_value = Pricing_Integral(0, z, 1000.0);
    return integral_value / sqrt(2.0 * pi);
}
double D(double x, double r, double sigma, double T)
{
    return (log(x) + (r + ((.5 * pow(sigma, 2)) * T))) / (sigma * sqrt(T));
}


double UpAndOutEuropeanCall::BSAnalyticalPrice(double S0, double r, double sigma)
{
    const double d1 = D((S0 / m_K), r, sigma, m_T);  const double d2 = d1 - (sigma * sqrt(m_T));
    const double d3 = D((S0 / m_B), r, sigma, m_T);  const double d4 = d3 - (sigma * sqrt(m_T));
    const double d5 = D((S0 / m_B), -r, sigma, m_T);  const double d6 = d5 - (sigma * sqrt(m_T));
    const double d7 = D(((S0 * m_K) / (m_B * m_B)), -r, sigma, m_T);  const double d8 = d7 - (sigma * sqrt(m_T));

    return S0 * ((N(d1) - N(d3) - pow((m_B / S0), ((1.0 + ((2.0 * r)) / pow(sigma, 2.0)))) * (N(d6) - N(d8)))) -
        ((m_K * exp(-r * m_T)) * (N(d2) - N(d4) - pow((m_B / S0), ((-1.0 + ((2.0 * r)) / pow(sigma, 2.0)))) * (N(d5) - N(d7))));
  
}

double UpAndOutEuropeanCall::payoff(double S)
{
    if (S > m_B || m_K > m_B) //if either value is greater than the barrier, the option is worthless
    {
        return 0;
    }
    else if (S - m_K > 0) //basic call payoff function: max(S-K, 0)
    {
        return S - m_K;
    }
    else
    {
        return 0;
    }
}
double UpAndOutEuropeanCall::BinomialTreePrice(double S0, double r, double sigma)
{
    const int N = 90; //90 seems to return the closest to actual price for our example
    const double time_step = m_T / N;
    const double u = exp((r - (pow(sigma, 2.0) / 2.0) * time_step + sigma * sqrt(time_step)));
    const double d = exp((r - pow(sigma, 2.0) / 2.0) * time_step - sigma * sqrt(time_step));
    const double R = exp(r * time_step) - 1.0;
    const double qu = (1 + R - d) / (u - d);
    const double qd = 1 - qu;

    vector<double> v(N + 1);
    for (int i = 0; i <= N; ++i)
    {
        const double ST = S0 * pow(u, i) * pow(d, N - i);
        v[i] = payoff(ST);
    }
    for (int n = N - 1; n >= 0; --n)
    {
        for (int i = 0; i <= n; ++i)
        {
            v[i] = (qu * v[i + 1] + qd * v[i]) / (1.0 + R);
        }
    }

    return v[0];
}