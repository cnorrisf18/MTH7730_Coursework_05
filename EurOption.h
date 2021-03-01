#pragma once
#include <cmath>
#include <string>


class EurOption
{
public:

	EurOption(double T_, double K_) { T = T_; K = K_; m_call_or_put = "NONE"; };
	EurOption(double T_, double K_, std::string cop) { T = T_; K = K_; m_call_or_put = cop; }
	virtual double PriceByBSFormula(double S0, double sigma, double r) { return 0; };
	double VegaByBSFormula(double S0,
		double sigma, double r);
	virtual double DeltaByBSFormula(double S0, double sigma, double r) { return 0; };
	virtual double ThetaByBSFormula(double S0, double sigma, double r) { return 0; };
	virtual double GammaByBSFormula(double S0, double sigma, double r);
	virtual double PriceByIntegral(double S0, double sigma, double r);
	void PrintValues(double S0, double sigma, double r);

protected:
	double d_plus(double S0, double sigma, double r);
	double d_minus(double S0, double sigma, double r);
	double T, K;
	double N(double x);
	double Nprime(double x);
	std::string m_call_or_put;
	
	
};
