
#include <iostream>
#include <math.h>
#include <stdexcept>
#include <sstream>
#include <string>
#include <math.h>

#include "LoanCalculator.h"

using namespace std;

LoanCalculator::LoanCalculator() :
  amountSet_(false),
  interestSet_(false),
  paymentSet_(false),
  periodTotalSet_(false),
  periodElapsedSet_(false),
  openingFee_(0.0),
  openingPercent_(0.0),
  initialPayment_(0.0)
{
}

//
// The actual calculation methods
//

/**
 * Loan balance after n payments have been made:
 *   B_n = A*(1+i)^n - (P/i)*((1+i)^n - 1)
 */
float LoanCalculator::calculateLoanBalance()
{
  if(!amountSet_ || !interestSet_ || !periodElapsedSet_ || !paymentSet_)
  {
    throw invalid_argument("Must set loan amount, interest, and elapsed period for this calculation" );
  }

  return (amount_*pow((1+interestPeriodic_), periodElapsed_)) -
         (payment_/interestPeriodic_)*(pow((1+interestPeriodic_), periodElapsed_)-1);
}

/**
 * Payment amount on a loan:
 *   P = i*A / (1 - (1+i)^-N)
 */
float LoanCalculator::calculatePayment()
{
  if(!amountSet_ || !interestSet_ || !periodTotalSet_)
  {
    throw invalid_argument("Must set loan amount, interest, and total period for this calculation" );
  }

  float totalAmount = amount_ - initialPayment_;
  totalAmount = totalAmount + openingFee_ + (totalAmount * (openingPercent_/100.0));

  return (interestPeriodic_*totalAmount) /
         (1 - pow((1+interestPeriodic_), (-1*periodTotal_)));
}

/**
 * Number of payments on a loan:
 *   N = -log(1-i*A/P) / log(1+i)
 *      (You can use any logarithm base, as long as both logs use the same base.)
 *      Aunt Sally offers to lend you $3500 at 6% for that new home theater system you want.
 *      If you pay her back $100 a month, how long will it take?
 *      Solution:  6% per year is 0.5% per month, or 0.005. P = 100 and A = 3500. N = 38.57
 */
float LoanCalculator::calculateNumberPayments()
{
  if(!amountSet_ || !interestSet_ || !paymentSet_)
  {
    throw invalid_argument("Must set loan amount, interest, and payment for this calculation" );
  }

  return (-1.0*log10(1.0-(interestPeriodic_*amount_/payment_))) /
         log10(1.0 + interestPeriodic_);
}

/**
 * Original loan amount:
 *   A = (P/i)*(1 - (1+i)^-N)
 */
float LoanCalculator::calculateLoanAmount()
{
  if(!paymentSet_ || !interestSet_ || !periodTotalSet_)
  {
    throw invalid_argument("Must set payment, interest, and total period for this calculation" );
  }

  return (payment_/interestPeriodic_) *
         (1 - pow((1+interestPeriodic_), (-1*periodTotal_)));
}

/**
 * Interest Rate:
 *   i = (((1 + P/A)^(1/q) - 1 )^q - 1)  NOTICE: This is an approximate not an exact solution
 *   where q = log(1+1/N) / log(2)
*/
float LoanCalculator::calculateInterestRate()
{
  if(!amountSet_ || !paymentSet_ || !periodTotalSet_)
  {
    throw invalid_argument("Must set amount, payment, and total period for this calculation" );
  }

  float q = log10(1.0 + 1.0/periodTotal_) / log10(2.0);
  float monthlyInterest = pow((pow((1.0 + payment_/amount_), 1.0/q) -1.0), q) -1.0;

  return monthlyInterest*12*100;
}

float LoanCalculator::calculateEffectiveInterestRate()
{
  if(!amountSet_ || !periodTotalSet_)
  {
    throw invalid_argument("Must set amount and total period for this calculation" );
  }

  float payment = calculatePayment();
  float totalAmount = amount_ - initialPayment_;

  float q = log10(1.0 + 1.0/periodTotal_) / log10(2.0);
  float monthlyInterest = pow((pow((1.0 + payment/totalAmount), 1.0/q) -1.0), q) -1.0;

  return monthlyInterest*12*100;
}

std::string LoanCalculator::toString()
{
  stringstream ss;

  //ss << "LoanCalculator set values:\n";

  if(amountSet_)
  {
    ss << "Initial Amount:      " << amount_ << "\n";
  }

  if(initialPayment_ != 0.0)
  {
    ss << "Initial Payment:     " << initialPayment_ << "\n";
    ss << "Actual Loan Amount:  " << (amount_ - initialPayment_) << "\n";
  }

  if(interestSet_)
  {
    ss << "Yearly Interest:     " << interest_ << "%\n";
    //ss << "Yearly Interest:     " << interest_
    //   << "\nMonthly Interest:    " << interestPeriodic_ << "\n";
  }

  if(paymentSet_)
  {
    ss << "Monthly payment:     " << payment_ << "\n";
  }

  if(periodTotalSet_)
  {
    ss << "Loan Period:         " << periodTotal_ << " months\n";
  }

  if(periodElapsedSet_)
  {
    ss << "Elapsed Period:      " << periodElapsed_ << " months\n";
  }

  if(openingFee_ != 0.0)
  {
      ss << "Opening Fee:       " << openingFee_ << "\n";
  }

  if(openingPercent_ != 0.0)
  {
    ss << "Opening Fee %:       " << openingPercent_ << "% = "
       << openingPercent_/100*(amount_ - initialPayment_) << "\n";
  }

  return ss.str();
}
