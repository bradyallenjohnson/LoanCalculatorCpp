
#include <stdlib.h>

#include <exception>
#include <iostream>
#include <stdexcept>
#include <string>

#include <QApplication>

#include <LoanCalcQtMainWindow.h>
#include <CmdLineParser.h>
#include <LoanCalculator.h>

using namespace std;

enum CALC_TYPE
{
  CALC_UNKNOWN=0,
  CALC_BALANCE=100,
  CALC_PAYMENT,
  CALC_NUMPAYMENTS,
  CALC_AMOUNT,
  CALC_INTEREST
};

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

void loadCmdLine(CmdLineParser &clp)
{
  clp.setMainHelpText("A simple loan calculator");
  clp.setMainHelpTextEnd("With no options set, a GUI will be launched");

  // Calculation types
  clp.addMutExclCmdLineOption(new CmdLineOptionFlag(ARG_CALC_BALANCE,
         "Calculate the loan balance after making several payments, given:\n"
         "\t\t loan amount, interest, monthly payment and number of monthly payments made so far",
         false, CALC_BALANCE));
  clp.addMutExclCmdLineOption(new CmdLineOptionFlag(ARG_CALC_PAYMENT,
         "Calculate the monthly loan payment, given: loan amount, loan period, and interest",
         false, CALC_PAYMENT));
  clp.addMutExclCmdLineOption(new CmdLineOptionFlag(ARG_CALC_NUMPAYMENTS,
         "Calculate the number of payments needed to pay a loan, given: loan amount, monthly payment, interest",
         false, CALC_NUMPAYMENTS));
  clp.addMutExclCmdLineOption(new CmdLineOptionFlag(ARG_CALC_AMOUNT,
         "Calculate the initial loan amount, given: monthly payment, loan period, and interest",
         false, CALC_AMOUNT));
  clp.addMutExclCmdLineOption(new CmdLineOptionFlag(ARG_CALC_INTEREST,
         "Calculate the loan interest, given: loan amount, loan period, and monthly payment",
         false, CALC_INTEREST));
  clp.setMutExclUsageText("Calculations");

  // Different values
  clp.addCmdLineOption(new CmdLineOptionFloat( ARG_PAYMENT, "Set the monthly loan payment. Ej: 325.67"));
  clp.addCmdLineOption(new CmdLineOptionInt(   ARG_PERIOD_TOTAL, "Set the total loan period in months. Ej: 60"));
  clp.addCmdLineOption(new CmdLineOptionInt(   ARG_PERIOD_ELAPSED, "Set the elapsed period in months. Ej: 32"));
  clp.addCmdLineOption(new CmdLineOptionInt(   ARG_AMOUNT, "Set the initial amount. Ej: 19300"));
  clp.addCmdLineOption(new CmdLineOptionFloat( ARG_INITIAL_PAYMENT,
         "Set the initial payment, loan will be for (initial amount - initial payment) Ej: 1000, Default 0.0"));
  clp.addCmdLineOption(new CmdLineOptionFloat( ARG_INTEREST, "Set the yearly interest rate. Ej: 6.75"));
  clp.addCmdLineOption(new CmdLineOptionFloat( ARG_OPENFEE, "Set fees for opening the loan. Ej: 100, Default 0.0"));
  clp.addCmdLineOption(new CmdLineOptionFloat( ARG_OPENPERCENT,
         "Set fees for opening the loan, charged as a percentage. Ej: 2.75%, Default 0.0%"));
  
  clp.setMinNumberArgs(3);
}

//
// Simple Command line parser
//
CALC_TYPE parseCommandLine(int argc, char **argv, CmdLineParser &clp, LoanCalculator &calculator)
{
  CALC_TYPE ct(CALC_UNKNOWN);

  if(!clp.parseCmdLine(argc, argv))
  {
    clp.printUsage();
    return ct;
  }

  calculator.setAmount(
       ((CmdLineOptionInt*)   clp.getCmdLineOption(ARG_AMOUNT))->getValue());
  calculator.setInitialPayment(
       ((CmdLineOptionFloat*) clp.getCmdLineOption(ARG_INITIAL_PAYMENT))->getValue());
  calculator.setInterest(
       ((CmdLineOptionFloat*) clp.getCmdLineOption(ARG_INTEREST))->getValue());
  calculator.setPayment(
       ((CmdLineOptionFloat*) clp.getCmdLineOption(ARG_PAYMENT))->getValue());
  calculator.setPeriodTotal(
       ((CmdLineOptionInt*)   clp.getCmdLineOption(ARG_PERIOD_TOTAL))->getValue());
  calculator.setPeriodElapsed(
       ((CmdLineOptionInt*)   clp.getCmdLineOption(ARG_PERIOD_ELAPSED))->getValue());
  calculator.setOpeningFee(
       ((CmdLineOptionFloat*) clp.getCmdLineOption(ARG_OPENFEE))->getValue());
  calculator.setOpeningPercent(
       ((CmdLineOptionFloat*) clp.getCmdLineOption(ARG_OPENPERCENT))->getValue());

  CmdLineOption *option(clp.getMutExclOption());
  if(option != NULL) // cant be NULL, else the parser mutExcl checking didnt work
  {
    ct = (CALC_TYPE) ((CmdLineOptionFlag*) option)->getValueKey();
  }

  return ct;
}

//
// Main program
//
int main(int argc, char **argv)
{
  LoanCalculator calculator;

  // If no arguments are given, then launch the GUI
  if(argc == 1)
  {
    QApplication app(argc, argv);

    LoanCalcQtMainWindow mainWindow(&calculator);
    mainWindow.show();

    return app.exec();
  }

  //
  // Parse the command line arguments
  //
  CmdLineParser clp;
  loadCmdLine(clp);
  CALC_TYPE ct = parseCommandLine(argc, argv, clp, calculator);

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
    else if(ct == CALC_UNKNOWN)
    {
      // most likely the case that help was selected
      return 1;
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
