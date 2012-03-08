
#include <exception>
#include <iostream>
#include <stdexcept>
#include <string>

#include <LoanCalculator.h>

using namespace std;

//
// Some commonly used constants
//
const string ARG_CALC_BALANCE      = "-cb";
const string ARG_CALC_PAYMENT      = "-cp";
const string ARG_CALC_NUMPAYMENTS  = "-cn";
const string ARG_CALC_AMOUNT       = "-ca";
const string ARG_CALC_INTEREST     = "-ci";

const string ARG_PAYMENT           = "-p";
const string ARG_PERIOD_TOTAL      = "-N";
const string ARG_PERIOD_ELAPSED    = "-n";
const string ARG_AMOUNT            = "-a";
const string ARG_INITIAL_PAYMENT   = "-ai";
const string ARG_INTEREST          = "-i";
const string ARG_OPENFEE           = "-of";
const string ARG_OPENPERCENT       = "-op";

const string ARG_HELP = "-h";

enum CALC_TYPE
{
  CALC_UNKNOWN=0,
  CALC_BALANCE=100,
  CALC_PAYMENT,
  CALC_NUMPAYMENTS,
  CALC_AMOUNT,
  CALC_INTEREST
};

// Help
void printUsage()
{
  cout << "\nUsage:\n"
       << "Input values:\n"
       << "\t " << ARG_AMOUNT << " Set the initial amount. Ej: 19300\n"
       << "\t " << ARG_INITIAL_PAYMENT << " Set the initial payment, loan will be for (initial amount - initial payment) Ej: 1000, Default 0.0\n"
       << "\t " << ARG_INTEREST << " Set the yearly interest rate. Ej: 6.75\n"
       << "\t " << ARG_PAYMENT << " Set the monthly loan payment. Ej: 325.67\n"
       << "\t " << ARG_PERIOD_TOTAL << " Set the total loan period in months. Ej: 60\n"
       << "\t " << ARG_PERIOD_ELAPSED << " Set the elapsed period in months. Ej: 32\n"
       << "\t " << ARG_OPENFEE << " Set fees for opening the loan. Ej: 100, Default 0.0\n"
       << "\t " << ARG_OPENPERCENT << " Set fees for opening the loan, charged as a percentage. Ej: 2.75%, Default 0.0%\n"

       << "Calculations, one of the following:\n"
       << "\t " << ARG_CALC_PAYMENT
       << " Calculate the monthly loan payment, given: loan amount, loan period, and interest\n"

       << "\t " << ARG_CALC_AMOUNT
       << " Calculate the initial loan amount, given: monthly payment, loan period, and interest\n"

       << "\t " << ARG_CALC_INTEREST
       << " Calculate the loan interest, given: loan amount, loan period, and monthly payment\n"

       << "\t " << ARG_CALC_NUMPAYMENTS
       << " Calculate the number of payments needed to pay a loan, given: loan amount, monthly payment, interest\n"

       << "\t " << ARG_CALC_BALANCE
       << " Calculate the loan balance after making several payments, given:\n"
       << "\t\t loan amount, interest, monthly payment and number of monthly payments made so far\n"

       << "Other options:\n"
       << "\t " << ARG_HELP << " Display this message\n"
       << endl;
}

//
// Simple Command line parser
//
CALC_TYPE parseCommandLine(int argc, char **argv, LoanCalculator &calculator)
{
  CALC_TYPE ct = CALC_UNKNOWN;

  if(argc < 3)
  {
    if(argc == 2 && ARG_HELP == argv[1])
    {
      printUsage();
      exit(1);
    }

    throw invalid_argument("Invalid number of command line arguments");
  }

  for(int i = 1; i < argc; i++)
  {
    string argvStr = argv[i];

       // input values
    if(ARG_AMOUNT == argvStr)
    {
      calculator.setAmount(atof(argv[++i]));
    }
    else if(ARG_INITIAL_PAYMENT == argvStr)
    {
      calculator.setInitialPayment(atof(argv[++i]));
    }
    else if(ARG_INTEREST == argvStr)
    {
      calculator.setInterest(atof(argv[++i]));
    }
    else if(ARG_PAYMENT == argvStr)
    {
      calculator.setPayment(atof(argv[++i]));
    }
    else if(ARG_PERIOD_TOTAL == argvStr)
    {
      calculator.setPeriodTotal(atoi(argv[++i]));
    }
    else if(ARG_PERIOD_ELAPSED == argvStr)
    {
      calculator.setPeriodElapsed(atoi(argv[++i]));
    }
    else if(ARG_OPENFEE == argvStr)
    {
      calculator.setOpeningFee(atof(argv[++i]));
    }
    else if(ARG_OPENPERCENT == argvStr)
    {
      calculator.setOpeningPercent(atof(argv[++i]));
    }

       // calculation types
    else if(ARG_CALC_AMOUNT == argvStr)
    {
      ct = CALC_AMOUNT;
    }
    else if(ARG_CALC_BALANCE == argvStr)
    {
      ct = CALC_BALANCE;
    }
    else if(ARG_CALC_INTEREST == argvStr)
    {
      ct = CALC_INTEREST;
    }
    else if(ARG_CALC_NUMPAYMENTS == argvStr)
    {
      ct = CALC_NUMPAYMENTS;
    }
    else if(ARG_CALC_PAYMENT == argvStr)
    {
      ct = CALC_PAYMENT;
    }

       // Other args
    else if(ARG_HELP == argvStr)
    {
      printUsage();
      exit(1);
    }
    else
    {
      throw invalid_argument("Invalid command line argument: " + argvStr );
    }
  }

  return ct;
}

//
// Main program
//
int main(int argc, char **argv)
{
  LoanCalculator calculator;
  CALC_TYPE ct;

  //
  // Parse the command line arguments
  //
  try
  {
    ct = parseCommandLine(argc, argv, calculator);
  }
  catch(const exception &e)
  {
    cerr << "Error parsing command line: " << e.what() << endl;
    printUsage();
    return 0;
  }

  try
  {
    cout << endl;

    // Not sure why I had to cast the result to float, but otherwise it printed strange results
    if(ct == CALC_BALANCE)
    {
        cout << "Loan Balance = " << (float) calculator.calculateLoanBalance() << endl;
    }
    else if(ct == CALC_PAYMENT)
    {
      float payment = calculator.calculatePayment();
      cout << "Monthly Payment    = " << payment << "\n"
           << "Total amt paid     = " << (float) (payment*calculator.getPeriodTotal())
           << endl;

      if(calculator.getOpeningPercent() != 0.0 || calculator.getOpeningFee() != 0.0)
      {
        cout << "Interest with fees = "
             << (float) calculator.calculateEffectiveInterestRate()
             << "%"
             << endl;
      }
    }
    else if(ct == CALC_NUMPAYMENTS)
    {
      cout << "Number of payments = " << (float) calculator.calculateNumberPayments() << endl;
    }
    else if(ct == CALC_AMOUNT)
    {
      cout << "Initial Loan amount = " << (float) calculator.calculateLoanAmount() << endl;
    }
    else if(ct == CALC_INTEREST)
    {
      cout << "Yearly Interest Rate = " << (float) calculator.calculateInterestRate() << "%" << endl;
    }
    else
    {
      cerr << "Unrecognized calculation type, exiting" << endl;
      return 0;
    }

    // print the values set on the calculator
    cout << calculator.toString() << endl;
  }
  catch(const exception &e)
  {
    cerr << "Error executing loan calculator: " << + e.what() << endl;
    //printUsage();
    //return 0;
  }

  cout << endl;
}
