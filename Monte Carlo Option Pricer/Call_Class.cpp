#include <iostream>
#include <algorithm>
#include <utility>
#include <string>
#include <cmath>
#include "Payoff_Option.h"

using namespace std;

int main() {
	//create an object for the class Call_Put_Option
	Call_Put_Option Option1;

	//define înput variables
	double Spot;
	double Strike;
	double Riskfree;
	double Vol;
	double Matu;
	double Simulnb;
	double Infbarrier;
	double Supbarrier;
	string OptionType;
	cout << "\nEnter the value of the Spot: " << endl;
	cin >> Spot;
	cout << "\nEnter the value of the Strike: " << endl;
	cin >> Strike;
	cout << "\nEnter the Risk Free Rate (in %) : " << endl;
	cin >> Riskfree;
	cout << "\nEnter the Volatily (in %) : " << endl;
	cin >> Vol;
	cout << "\nEnter the maturity (in years) : " << endl;
	cin >> Matu;
	cout << "\nEnter the wanted number of simulation : " << endl;
	cin >> Simulnb;
	cout << "\nWhat type of option? (European,Digital,Barrier,Asian,Lookback) " << endl;
	cin >> OptionType;

	//collect input data
	if (OptionType == "Barrier") {
		cout << "\nEnter the Inf Barrier : " << endl;
		cin >> Infbarrier;
		cout << "\nEnter the Sup Barrier : " << endl;
		cin >> Supbarrier;
		Option1.setStockPrice(Spot);
		Option1.setStrikePrice(Strike);
		Option1.setRiskFreeRate(Riskfree / 100);
		Option1.setVolatility(Vol / 100);
		Option1.setMaturity(Matu);
		Option1.setSimulationNumber(Simulnb);
		Option1.setBarrierInf(Infbarrier);
		Option1.setBarrierSup(Supbarrier);
		Option1.setOptionType(OptionType); //European/Digital/Barrier/Asian/Lookback
	}
	else {
		Option1.setStockPrice(Spot);
		Option1.setStrikePrice(Strike);
		Option1.setRiskFreeRate(Riskfree / 100);
		Option1.setVolatility(Vol / 100);
		Option1.setMaturity(Matu);
		Option1.setSimulationNumber(Simulnb);
		Option1.setOptionType(OptionType);
	}

	//call class method
	Option1.monte_carlo_price();
}