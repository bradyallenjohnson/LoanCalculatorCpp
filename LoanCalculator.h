#ifndef LOANCALCULATOR_H_INCLUDED
#define LOANCALCULATOR_H_INCLUDED

/*
Formulas from: http://oakroadsystems.com/math/loan.htm

Loan balance after n payments have been made:
  B_n = A*(1+i)^n - (P/i)*((1+i)^n - 1)


Payment amount on a loan:
  P = i*A / (1 - (1+i)^-N)


Number of payments on a loan: (You can use any logarithm base, as long as both logs use the same base.)
                              Aunt Sally offers to lend you $3500 at 6% for that new home theater system you want.
                              If you pay her back $100 a month, how long will it take?
                              Solution:  6% per year is 0.5% per month, or 0.005. P = 100 and A = 3500. N = 38.57
  N = -log(1-i*A/P) / log(1+i)


Original loan amount:
  A = (P/i)*(1 - (1+i)^-N)


Interest Rate:
  i = (((1 + P/A)^(1/q) - 1 )^q - 1)  NOTICE: This is an approximate not an exact solution
  where q = log(1+1/N) / log(2)


Variables:
A   	the loan amount (the principal sum) or initial investment
B_n or Bn   	(pronounced B sub n) the balance after n payments have been made. After the last payment has been made, B_N is zero.)
i   	the interest rate per period, not per year
      (For instance, if the loan payments are made monthly and the interest rate is 9%, then i = 9%/12 = 0.75% = 0.0075.)
n   	the number of time periods elapsed at any given point
N   	the total number of payments for the entire loan or investment
P   	the amount of each equal payment
*/

#include <string>

class LoanCalculator
{
public:
  LoanCalculator();
  ~LoanCalculator();

  //
  // Setters and Getters
  //

  /**
   * Total loan amount A
   */
  void setAmount(float A);
  float getAmount();

  /**
   * Initial down payment
   */
  void setInitialPayment(float initialA);
  float getInitialPayment();

  /**
   * Yearly interest rate i as in 6.75
   * Internally .0675/12 will be used
   * If 6.75 is passed to setInterest()
   *    getInterest() will return 6.75
   *    getPeriodicInterest() will return .0675/12.0
   */
  void setInterest(float i);
  float getInterest();
  float getPeriodicInterest();

  void setPayment(float P);
  float getPayment();

  void setPeriodTotal(int N);
  int getPeriodTotal();

  void setPeriodElapsed(int n);
  int getPeriodElapsed();

  void setOpeningFee(float fee);
  float getOpeningFee();

  void setOpeningPercent(float percent);
  float getOpeningPercent();

  //
  // The actual calculation methods
  //

  float calculateLoanBalance();
  float calculatePayment();
  float calculateNumberPayments();
  float calculateLoanAmount();
  float calculateInterestRate();
  // The effective interest rate, once fees have been applied
  float calculateEffectiveInterestRate();

  std::string toString();

private:
  float amount_;        // loan amount
  bool amountSet_;

  float initialPayment_;     // initial down payment

  float interest_;          // interest rate, something like 6.75
  float interestPeriodic_;  // this will be .0675/12
  bool interestSet_;

  float payment_;       // payment amount
  bool paymentSet_;

  int periodTotal_;     // total payment periods
  bool periodTotalSet_;

  int periodElapsed_;   // number of elapsed payment periods
  bool periodElapsedSet_;

  // These two are used if loans charge a fee opening fee or percentage
  float openingFee_;
  float openingPercent_;

};

#endif // LOANCALCULATOR_H_INCLUDED
