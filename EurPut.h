#include "EurOption.h"
#include <string>
#pragma once
#ifndef EurPut_h
#define EurPut_h

class EurPut : public EurOption
{
public:

    EurPut(double T_, double K_) : EurOption(T_, K_, "PUT") {};
    EurPut(double T_, double K_, std::string cop) : EurOption(T_, K_, cop) {};
    double PriceByBSFormula(double S0,
        double sigma, double r);
    virtual double DeltaByBSFormula(double S0, double sigma, double r);
    virtual double ThetaByBSFormula(double S0, double sigma, double r);

};

#endif
