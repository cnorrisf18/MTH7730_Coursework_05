#include "EurOption.h"
#ifndef EurCall_h
#define EurCall_h
#include <string>

class EurCall: public EurOption
{

   public:   
       EurCall(double T_, double K_) : EurOption(T_, K_, "CALL") {};
       EurCall(double T_, double K_, std::string cop) : EurOption(T_, K_, cop) {};
      virtual double PriceByBSFormula(double S0,
         double sigma, double r);
      virtual double DeltaByBSFormula(double S0, double sigma, double r);
      virtual double ThetaByBSFormula(double S0, double sigma, double r);
};

#endif
