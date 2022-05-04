#pragma once

#include <string>

using namespace std;

class Call_Put_Option
{
private:
	double Stock_Price;
	double Strike_Price; 
	double Risk_Free_Rate;
	double Volatility;
	double Maturity;
	double Simulation_Number;
	double Barrier_Inf;
	double Barrier_Sup;
	string Option_Type;

public:
	//constructors and destructors
	Call_Put_Option(double inStockPrice=100.0 , double inStrikePrice=100.0 ,double inRiskFreeRate=0.05, \
		double inVolatility=0.2, double inMaturity=1.0, double inSimulationNumber=10000000, double inBarrierInf=90, \
		double inBarrierSup=110, string inOptionType="European");
	~Call_Put_Option();
	//getters & setters
	void setStockPrice(double Stock_Price) { (*this).Stock_Price = Stock_Price; }
	double getStockPrice() { return Stock_Price; }
	void setStrikePrice(double Strike_Price) { (*this).Strike_Price = Strike_Price; }
	double getStrikePrice() { return Strike_Price; }
	void setRiskFreeRate(double Risk_Free_Rate) { (*this).Risk_Free_Rate = Risk_Free_Rate; }
	double getRiskFreeRate() { return Risk_Free_Rate; }
	void setVolatility(double Volatility) { (*this).Volatility = Volatility; }
	double getVolatility() { return Volatility; }
	void setMaturity(double Maturity) { (*this).Maturity = Maturity; }
	double getMaturity() { return Maturity; }
	void setSimulationNumber(double Simulation_Number) { (*this).Simulation_Number = Simulation_Number; }
	double getSimulationNumber() { return Simulation_Number; }
	void setOptionType(string Option_Type) { (*this).Option_Type = Option_Type; }
	string getOptionType() { return Option_Type; }
	void setBarrierInf(double Barrier_Inf) { (*this).Barrier_Inf = Barrier_Inf; }
	double getBarrierInf() { return Barrier_Inf; }
	void setBarrierSup(double Barrier_Sup) { (*this).Barrier_Sup = Barrier_Sup; }
	double getBarrierSup() { return Barrier_Sup; }

	//methods
	void monte_carlo_price();
};

