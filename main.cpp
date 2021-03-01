#include "UpAndOutEuropeanCall.h"
#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include "EurCall.h"
#include "EurPut.h"
using namespace std;

void NegativeMessage(double& x, string x_name)
{
	cout << endl << x_name << " should not be negative, please enter a new value. ";
	cin >> x;
}

void GetMarketInputs(double& spot_price, double& volatility, double& interest_rate)
{
	//get market inputs
	cout << endl << "Please enter the current price of the stock: ";
	cin >> spot_price;
	cout << endl << "Please enter volatility: ";
	cin >> volatility;
	cout << endl << "Please enter the continuously-compounded risk-free interest rate (as a decimal): ";
	cin >> interest_rate;
	//check inputs
	bool done = false;
	while (not done)
	{
		if (spot_price < 0)
		{
			NegativeMessage(spot_price, "Stock price");
		}
		else if (volatility < 0)
		{
			NegativeMessage(volatility, "Volatility");
		}
		else if (interest_rate < 0)
		{
			NegativeMessage(interest_rate, "Interest rate");
		}
		else if (interest_rate > 1)
		{
			string Confirmation;
			cout << endl << "The entered interest rate is higher than 1 (100%), are you sure this is correct? type YES or NO ";
			cin >> Confirmation;
			if (Confirmation == "NO")
			{
				cout << endl << "Please enter a new value for the interest rate. ";
				cin >> interest_rate;
			}
			else if (Confirmation == "YES")
			{
				done = true;
			}
		}
		else
		{
			done = true;
		}
	}
}

void CalculateWeightedValues(vector<EurOption*> OptionVector, vector<int> QuantityVector, double S0, double sigma, double r)
{
	if (OptionVector.size() != QuantityVector.size()) //something messed up in the file read; they should have the same length
	{
		return;
	}
	else
	{
		double weighted_portfolio = 0; double weighted_delta = 0; double weighted_gamma = 0; double weighted_theta = 0;
		for (int i = 0; i < OptionVector.size(); i++)
		{
			weighted_portfolio += OptionVector[i]->PriceByBSFormula(S0, sigma, r) * QuantityVector[i];
			weighted_delta += OptionVector[i]->DeltaByBSFormula(S0, sigma, r) * QuantityVector[i];
			weighted_gamma += OptionVector[i]->GammaByBSFormula(S0, sigma, r) * QuantityVector[i];
			weighted_theta += OptionVector[i]->ThetaByBSFormula(S0, sigma, r) * QuantityVector[i];
		}
		cout << "Values for the entire portfolio: " << endl;
		cout << "Portfolio value: " << weighted_portfolio << endl;
		cout << "Delta value: " << weighted_delta << endl;
		cout << "Gamma value: " << weighted_gamma << endl;
		cout << "Theta value: " << weighted_theta << endl;
	}
	return;
}

int main1() {
	const double S0 = 98.0;
	const double r = .05;
	const double sigma = .2;
	//Question 1, part B
	UpAndOutEuropeanCall One_B(1.0, 100.0, 130.0);
	cout << "Exact Analytical Price: " << One_B.BSAnalyticalPrice(S0, r, sigma) << endl;

	//Question 1, part D
	
	cout << "Binomial tree pricer price: " << One_B.BinomialTreePrice(S0, r, sigma) << endl;

	return 0;

}

int main2() {
	
	vector<EurOption*> Portfolio;
	vector<int> quantities;
	ifstream port_file("PortfolioDetails.txt");
	if (!port_file.is_open())
	{
		cout << "Error opening file" << endl;
		return 0;
	}
	int i = 0;
	while (!port_file.eof())
	{
		string option_type;
		int quantity;
		double strike_price;
		double time_to_expiry;
		port_file >> option_type >> quantity >> strike_price >> time_to_expiry;
		//cout << option_type << " " << quantity << " " << strike_price << " " << time_to_expiry << endl;

		if (option_type == "C")
		{
			EurOption* OptionPointer = new EurCall(time_to_expiry, strike_price);
			Portfolio.push_back(OptionPointer);
			//Portfolio.push_back(EurCall*(time_to_expiry, strike_price));
		}
		else if (option_type == "P")
		{
			EurOption* OptionPointer = new EurPut (time_to_expiry, strike_price);
			Portfolio.push_back(OptionPointer);
			//Portfolio.push_back(EurPut*(time_to_expiry, strike_price));
		}
		else
		{
			cout << "Wrong option type entered: The options are C and P" << endl;
		}
		quantities.push_back(quantity);
		++i;
	}
	port_file.close();
	double spot_price; double volatility; double interest_rate;
	GetMarketInputs(spot_price, volatility, interest_rate);
	for (int z = 0; z < Portfolio.size(); ++z)
	{
		cout << endl <<  "For option " << z + 1 << " in the portfolio: " << endl;
		Portfolio[z]->PrintValues(spot_price, volatility, interest_rate);
		cout << endl << endl;
	}
	CalculateWeightedValues(Portfolio, quantities, spot_price, volatility, interest_rate);
	return 0;
}

int main()
{
	int question = 2;
	cout << "What question do you want to run? ";
	cin >> question;
	switch (question)
	{
	case 1:
		main1();
		return 0;
	case 2:
		main2();
		return 0;
	default:
		main2();
		return 0;
	}
}