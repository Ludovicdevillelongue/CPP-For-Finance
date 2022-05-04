#include "Payoff_Option.h"
#include "GreeksCP.h"
#include <algorithm>
#include <utility>
#include <iostream>
#include <tuple>


using namespace std;

//constructor call
Call_Put_Option::Call_Put_Option(double inStockPrice, double inStrikePrice, \
	double inRiskFreeRate, double inVolatility, double inMaturity, \
	double inSimulationNumber, double inBarrierInf, double inBarrierSup, string inOptionType)
{
	Stock_Price=inStockPrice;
	Strike_Price=inStrikePrice;
	Risk_Free_Rate= inRiskFreeRate;
	Volatility= inVolatility;
	Maturity=inMaturity;
	Simulation_Number=inSimulationNumber;
	Barrier_Inf = inBarrierInf;
	Barrier_Sup = inBarrierSup;

}

//destructor call
Call_Put_Option::~Call_Put_Option()
{
}


/*implementation of the Box Muller algorithm, used to generate
gaussian random numbers, necessary for the Monte Carlo method*/
double random_number_generator()
{
	double x = 0.0;
	double y = 0.0;
	double euclid_sq = 0.0;

  /*continue generating two uniform random variables
   until the square of their euclidean distance 
   is less than unity*/
	do {
		x = 2.0 * rand() / static_cast<double>(RAND_MAX) - 1;
		y = 2.0 * rand() / static_cast<double>(RAND_MAX) - 1;
		euclid_sq = x * x + y * y;
	} while (euclid_sq >= 1.0);

	return x * sqrt(-2 * log(euclid_sq) / euclid_sq);
}
/*heaviside returns unity when val
is greater than or equal to zero and returns 
zero otherwise to price digital option*/
double heaviside(const double& val) {
  if (val >= 0) {
      return 1.0;
  } else {
    return 0.0;
  }
}
/*pos_par returns the positive part of a 
given varible*/
double pos_par(double x)   
{
	if (x > 0) return x;
	return 0;
}
void Call_Put_Option::monte_carlo_price()
{
	double S_adjust = Stock_Price * exp(Maturity * (Risk_Free_Rate - 0.5 * Volatility * Volatility)); // Spot Price Adjustment 
	double S_cur = 0.0; //initialize spot
	double payoff_sum_call = 0.0; //intitialize payoff call
	double payoff_sum_put = 0.0; //initialize payoff put
	double call_price = 0.0; //initialize call price
	double put_price = 0.0; //initialize put price
	double NT = 10;
	double dt = Maturity / NT;
	double call_delta = 0.0; //initialize call delta no need for put delta as it is call delta - 1
	double call_gamma = 0.0; //initialize call gamma no need for put gamma as it is equal to call gamma
	double call_vega = 0.0; //initialize call vega no need for put vega as it is equal to call vega
	double call_theta = 0.0; //initialize call theta 
	double put_theta = 0.0; //initialize put theta
	double call_rho = 0.0; //initialize call rho
	double put_rho = 0.0; //initialize put rho

	for (int i = 0; i < Simulation_Number; i++) {
		double gauss_bm = random_number_generator(); //generate guassian random number
		S_cur = S_adjust * exp(sqrt(Volatility * Volatility * Maturity) * gauss_bm); //adjust spot
		if (Option_Type == "European") {
			payoff_sum_call += std::max(S_cur - Strike_Price, 0.0); //cumulate payoff
			payoff_sum_put += std::max(Strike_Price - S_cur, 0.0); //cumulate payoff
		}
		else if (Option_Type == "Asian") {
			double Snext = 0;
			double averageSum = 0;

			for (double t = 0; t <= Maturity; t += dt) { //inner loop for constructing one stock price path from time 0 to maturity
				Snext = S_cur * std::exp(-0.5*Volatility * Volatility*dt + Volatility * std::sqrt(dt) * gauss_bm);
				averageSum += Snext;
				S_cur = Snext;
			}

			payoff_sum_call += std::max(pos_par(averageSum / NT - Strike_Price), 0.0); //cumulate payoff
			payoff_sum_put += std::max(pos_par(Strike_Price - averageSum / NT), 0.0); //cumulate payoff
		}
		else if (Option_Type == "Lookback") {;
			double Snext = 0;
			double currentMin = S_cur; //set current minimum stock level to initial price

			for (double t = 0; t <= Maturity; t += dt) //inner loop for constructing one stock price from time 0 to T
			{
				Snext = S_cur * std::exp(-0.5*Volatility*Volatility*dt + Volatility * std::sqrt(dt)*gauss_bm);
				if (Snext < currentMin) currentMin = Snext;
				S_cur = Snext;
			}
			payoff_sum_call += std::max(pos_par(Snext - currentMin), 0.0); //cumulate payoff
			payoff_sum_put += std::max(pos_par(currentMin - Snext), 0.0); //cumulate payoff

		}
		else if (Option_Type == "Digital") {
			payoff_sum_call += heaviside(S_cur - Strike_Price); //cumulate payoff
			payoff_sum_put += heaviside(Strike_Price - S_cur); //cumulate payoff
		}
		else if(Option_Type == "Barrier") {
			if (S_cur > Barrier_Sup) {
				payoff_sum_call += std::max(Barrier_Sup - Strike_Price, 0.0); //cumulate payoff
				payoff_sum_put += std::max(Strike_Price - Barrier_Sup, 0.0); //cumulate payoff
			}
			else if (S_cur < Barrier_Inf) {
				payoff_sum_call += std::max(Barrier_Inf - Strike_Price, 0.0); //cumulate payoff
				payoff_sum_put += std::max(Strike_Price - Barrier_Inf, 0.0); //cumulate payoff
			}
			else {
				payoff_sum_call += std::max(S_cur - Strike_Price, 0.0); //cumulate payoff
				payoff_sum_put += std::max(Strike_Price - S_cur, 0.0); //cumulate payoff
			}
			

		}
	}
	//payoff per path with risk free rate discount
	call_price = (payoff_sum_call / static_cast<double>(Simulation_Number)) * exp(-Risk_Free_Rate * Maturity);
	put_price = (payoff_sum_put / static_cast<double>(Simulation_Number)) * exp(-Risk_Free_Rate * Maturity);

	//greeks calculation
	call_delta = delta_call(1, Stock_Price, Strike_Price, Risk_Free_Rate, Volatility, Maturity);
	call_gamma = gamma_call(Stock_Price, Strike_Price, Risk_Free_Rate, Volatility, Maturity);
	call_vega = vega_call(Stock_Price, Strike_Price, Risk_Free_Rate, Volatility, Maturity) / 100;
	call_theta = theta_call(Stock_Price, Strike_Price, Risk_Free_Rate, Volatility, Maturity);
	put_theta = theta_put(Stock_Price, Strike_Price, Risk_Free_Rate, Volatility, Maturity);
	call_rho = rho_call(Stock_Price, Strike_Price, Risk_Free_Rate, Volatility, Maturity);
	put_rho = rho_put(Stock_Price, Strike_Price, Risk_Free_Rate, Volatility, Maturity);

	//console output
	cout << "\n\n Monte Carlo " << Option_Type << " Option Pricing" << endl;
	cout << "----------------------------------" << endl;
	cout << "\n Call Price: " << call_price << endl;
	cout << " Put Price: " << put_price <<"\n" << endl;
	cout << "----------------------------------" << endl;
	cout << "\n Greeks for European Call \n" << endl;
	cout << "Call Delta: " << call_delta << endl;
	cout << "Call Gamma: " << call_gamma << endl;
	cout << "Call Vega: " << call_vega << endl;
	cout << "Call Theta: " << call_theta << endl;
	cout << "Call Rho: " << call_rho << endl;
	cout << "----------------------------------" << endl;
	cout << "\n Greeks for European Put \n" << endl;
	cout << "Put Delta: " << call_delta - 1 << endl;
	cout << "Put Gamma: " << call_gamma << endl;
	cout << "Put Vega: " << call_vega << endl;
	cout << "Put Theta: " << put_theta << endl;
	cout << "Put Rho: " << put_rho << endl;
}