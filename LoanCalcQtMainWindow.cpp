
#include <QtGui>

#include "LoanCalculator.h"
#include "LoanCalcQtMainWindow.h"

LoanCalcQtMainWindow::LoanCalcQtMainWindow(LoanCalculator *calculator) : calculator_(calculator)
{
  mainWidget_ = new QWidget();
  setCentralWidget(mainWidget_);

  mainLayout_ = new QVBoxLayout;
  mainLayout_->setMargin(5);
  mainLayout_->addWidget(createCalcTypeOptions());
  mainLayout_->addWidget(createInputFields());
  mainLayout_->addWidget(createCalcResults());
  mainLayout_->addWidget(createButtons());
  mainWidget_->setLayout(mainLayout_);

  // MonthlyPayment is the default radio,
  // update the input fields accordingly
  pressedRadioMonthlyPayment();

  setWindowTitle(tr("Loan Calculator"));
}

LoanCalcQtMainWindow::~LoanCalcQtMainWindow()
{
  deleteCalcTypeOptions();
  deleteInputFields();
  deleteCalcResults();
  deleteButtons();
}

// Get the input fields and set the values on the calculator
void LoanCalcQtMainWindow::getInputFields()
{
  calculator_->reset();

  QString value = lineEditAmount_->text();
  if(!value.isEmpty()) {
    calculator_->setAmount(value.toFloat());
  }

  value = lineEditInitialPayment_->text();
  if(!value.isEmpty()) {
    calculator_->setInitialPayment(value.toFloat());
  }

  value = lineEditLoanFeePercent_->text();
  if(!value.isEmpty()) {
    calculator_->setOpeningPercent(value.toFloat());
  }

  value = lineEditInterest_->text();
  if(!value.isEmpty()) {
    calculator_->setInterest(value.toFloat());
  }

  value = lineEditPayment_->text();
  if(!value.isEmpty()) {
    calculator_->setPayment(value.toFloat());
  }

  value = lineEditMonths_->text();
  if(!value.isEmpty()) {
    calculator_->setPeriodTotal(value.toInt());
  }
}


//
// Private Object creation methods, only called from the constructor
//

QWidget *LoanCalcQtMainWindow::createCalcTypeOptions()
{
  radioMonthlyPayment_ = new QRadioButton(tr("&Monthly Payment"));
  radioAmount_         = new QRadioButton(tr("&Amount"));
  radioInterest_       = new QRadioButton(tr("&Interest"));
  radioNumPayments_    = new QRadioButton(tr("&Number of Payments"));
  radioBalance_        = new QRadioButton(tr("&Balance"));
  radioMonthlyPayment_->setChecked(true);

  layoutVboxCalcType_ = new QVBoxLayout();
  layoutVboxCalcType_->addWidget(radioMonthlyPayment_);
  layoutVboxCalcType_->addWidget(radioAmount_);
  layoutVboxCalcType_->addWidget(radioInterest_);
  layoutVboxCalcType_->addWidget(radioNumPayments_);
  layoutVboxCalcType_->addWidget(radioBalance_);

  groupBoxCalcType_ = new QGroupBox(tr("Calculation Type"));
  groupBoxCalcType_->setLayout(layoutVboxCalcType_);

  // Connect the radio signals
  connect(radioMonthlyPayment_, SIGNAL(pressed()), this, SLOT(pressedRadioMonthlyPayment()));
  connect(radioAmount_,         SIGNAL(pressed()), this, SLOT(pressedRadioAmount()));
  connect(radioInterest_,       SIGNAL(pressed()), this, SLOT(pressedRadioInterest()));
  connect(radioNumPayments_,    SIGNAL(pressed()), this, SLOT(pressedRadioNumPayments()));
  connect(radioBalance_,        SIGNAL(pressed()), this, SLOT(pressedRadioBalance()));

  return groupBoxCalcType_;
}

QWidget *LoanCalcQtMainWindow::createInputFields()
{
  labelAmount_         = new QLabel(tr("Amount"));
  labelInitialPayment_ = new QLabel(tr("Initial Payment"));
  labelLoanFeePercent_ = new QLabel(tr("Loan Fee%"));
  labelInterest_       = new QLabel(tr("Interest%"));
  labelPayment_        = new QLabel(tr("Payment"));
  labelMonths_         = new QLabel(tr("Months"));

  lineEditAmount_         = new QLineEdit();
  lineEditInitialPayment_ = new QLineEdit();
  lineEditLoanFeePercent_ = new QLineEdit();
  lineEditInterest_       = new QLineEdit();
  lineEditPayment_        = new QLineEdit();
  lineEditMonths_         = new QLineEdit();

  doubleValidator_ = new QDoubleValidator(this);
  intValidator_ = new QIntValidator(this);
  lineEditAmount_->setValidator(doubleValidator_);
  lineEditInitialPayment_->setValidator(doubleValidator_);
  lineEditLoanFeePercent_->setValidator(doubleValidator_);
  lineEditInterest_->setValidator(doubleValidator_);
  lineEditPayment_->setValidator(doubleValidator_);
  lineEditMonths_->setValidator(intValidator_);

  // When the keyboard shortcut is pressed, transfer focus to the buddy
  labelAmount_->setBuddy(lineEditAmount_);
  labelInitialPayment_->setBuddy(lineEditInitialPayment_);
  labelLoanFeePercent_->setBuddy(lineEditLoanFeePercent_);
  labelInterest_->setBuddy(lineEditInterest_);
  labelPayment_->setBuddy(lineEditPayment_);
  labelMonths_->setBuddy(lineEditMonths_);

  layoutGridInputFields_ = new QGridLayout();
  layoutGridInputFields_->addWidget(labelAmount_,             0, 0);
  layoutGridInputFields_->addWidget(lineEditAmount_,          0, 1);
  layoutGridInputFields_->addWidget(labelInitialPayment_,     1, 0);
  layoutGridInputFields_->addWidget(lineEditInitialPayment_,  1, 1);
  layoutGridInputFields_->addWidget(labelLoanFeePercent_,     2, 0);
  layoutGridInputFields_->addWidget(lineEditLoanFeePercent_,  2, 1);
  layoutGridInputFields_->addWidget(labelInterest_,           3, 0);
  layoutGridInputFields_->addWidget(lineEditInterest_,        3, 1);
  layoutGridInputFields_->addWidget(labelPayment_,            4, 0);
  layoutGridInputFields_->addWidget(lineEditPayment_,         4, 1);
  layoutGridInputFields_->addWidget(labelMonths_,             5, 0);
  layoutGridInputFields_->addWidget(lineEditMonths_,          5, 1);

  groupBoxInputFields_ = new QGroupBox(tr("Input Fields"));
  groupBoxInputFields_->setLayout(layoutGridInputFields_);

  return groupBoxInputFields_;
}

QWidget *LoanCalcQtMainWindow::createCalcResults()
{
  textEditCalcResults_ = new QTextEdit();
  textEditCalcResults_->setReadOnly(true);

  layoutVboxCalcResults_ = new QVBoxLayout();
  layoutVboxCalcResults_->addWidget(textEditCalcResults_);

  groupBoxCalcResults_ = new QGroupBox(tr("Calculation Results"));
  groupBoxCalcResults_->setLayout(layoutVboxCalcResults_);

  return groupBoxCalcResults_;
}

QWidget *LoanCalcQtMainWindow::createButtons()
{
  buttonCalculate_    = new QPushButton(tr("&Calculate"));
  buttonClearEntries_ = new QPushButton(tr("Clear &Entries"));

  layoutHboxButtons_ = new QHBoxLayout();
  layoutHboxButtons_->addWidget(buttonCalculate_);
  layoutHboxButtons_->addWidget(buttonClearEntries_);

  groupBoxButtons_ = new QGroupBox();
  groupBoxButtons_->setFlat(true);
  groupBoxButtons_->setLayout(layoutHboxButtons_);

  // Connect the button signals
  connect(buttonCalculate_,    SIGNAL(pressed()), this, SLOT(pressedButtonCalculate()));
  connect(buttonClearEntries_, SIGNAL(pressed()), this, SLOT(pressedButtonClearEntries()));

  return groupBoxButtons_;
}

//
// Private Object deletion methods, only called from the destructor
//
void LoanCalcQtMainWindow::deleteCalcTypeOptions()
{
  delete radioMonthlyPayment_;
  delete radioAmount_;
  delete radioInterest_;
  delete radioNumPayments_;
  delete radioBalance_;

  delete layoutVboxCalcType_;
  delete groupBoxCalcType_;
}

void LoanCalcQtMainWindow::deleteInputFields()
{
  delete labelAmount_;
  delete labelInitialPayment_;
  delete labelLoanFeePercent_;
  delete labelInterest_;
  delete labelPayment_;
  delete labelMonths_;

  delete lineEditAmount_;
  delete lineEditInitialPayment_;
  delete lineEditLoanFeePercent_;
  delete lineEditInterest_;
  delete lineEditPayment_;
  delete lineEditMonths_;

  delete layoutGridInputFields_;
  delete groupBoxInputFields_;
  delete doubleValidator_;
  delete intValidator_;
}

void LoanCalcQtMainWindow::deleteCalcResults()
{
  delete textEditCalcResults_;
  delete layoutVboxCalcResults_;
  delete groupBoxCalcResults_;
}

void LoanCalcQtMainWindow::deleteButtons()
{
  delete buttonCalculate_;
  delete buttonClearEntries_;

  delete layoutHboxButtons_;
  delete groupBoxButtons_;
}

//
// SLOTS
//

//
// Radio Button SLOTs
//

void LoanCalcQtMainWindow::enableFields(bool enableAmount,
                                        bool enableInitialPayment,
                                        bool enableLoanFee,
                                        bool enableInterest,
                                        bool enablePayment,
                                        bool enableMonths)
{
  // Set the labels accordingly
  labelAmount_->setEnabled(enableAmount);
  labelInitialPayment_->setEnabled(enableInitialPayment);
  labelLoanFeePercent_->setEnabled(enableLoanFee);
  labelInterest_->setEnabled(enableInterest);
  labelPayment_->setEnabled(enablePayment);
  labelMonths_->setEnabled(enableMonths);

  // Set the textEdit fields accordingly
  lineEditAmount_->setEnabled(enableAmount);
  lineEditInitialPayment_->setEnabled(enableInitialPayment);
  lineEditLoanFeePercent_->setEnabled(enableLoanFee);
  lineEditInterest_->setEnabled(enableInterest);
  lineEditPayment_->setEnabled(enablePayment);
  lineEditMonths_->setEnabled(enableMonths);
}

void LoanCalcQtMainWindow::pressedRadioMonthlyPayment()
{
  textEditCalcResults_->clear();
  enableFields(true,   // Amount
               true,   // InitialPayment
               true,   // LoanFeePercent
               true,   // Interest
               false,  // Payment
               true);  // Months
}

void LoanCalcQtMainWindow::pressedRadioAmount()
{
  textEditCalcResults_->clear();
  enableFields(false,  // Amount
               false,  // InitialPayment
               false,  // LoanFeePercent
               true,   // Interest
               true,   // Payment
               true);  // Months
}

void LoanCalcQtMainWindow::pressedRadioInterest()
{
  textEditCalcResults_->clear();
  enableFields(true,   // Amount
               false,  // InitialPayment
               false,  // LoanFeePercent
               false,  // Interest
               true,   // Payment
               true);  // Months
}

void LoanCalcQtMainWindow::pressedRadioNumPayments()
{
  textEditCalcResults_->clear();
  enableFields(true,   // Amount
               false,  // InitialPayment
               false,  // LoanFeePercent
               true,   // Interest
               true,   // Payment
               false); // Months
}

void LoanCalcQtMainWindow::pressedRadioBalance()
{
  textEditCalcResults_->clear();
  enableFields(true,   // Amount
               true,   // InitialPayment
               true,   // LoanFeePercent
               true,   // Interest
               true,   // Payment
               true);  // Months
}

//
// Push Button SLOTs
//

void LoanCalcQtMainWindow::pressedButtonCalculate()
{
  getInputFields(); // resets the calculator and set it with input fields
  QString result;

  if(radioMonthlyPayment_->isChecked()) {
    double payment = calculator_->calculatePayment();
    result.append("Monthly Payment = ");
    result.append(QString::number((double) payment, 'f', 2)); // 'f' is the float format, with 2 decimal places
    result.append("\nTotal amt paid = ");
    result.append(QString::number((double) (payment*calculator_->getPeriodTotal()), 'f', 2));

    if(calculator_->getOpeningPercent() != 0.0 || calculator_->getOpeningFee() != 0.0)
    {
      result.append("\nInterest with fees = ");
      result.append(QString::number((double) calculator_->calculateEffectiveInterestRate(), 'f', 2));
      result.append("%");
    }
  }
  else if(radioAmount_->isChecked()) {
    result.append("Initial Loan amount = ");
    result.append(QString::number((double) calculator_->calculateLoanAmount(), 'f', 2));
  }
  else if(radioInterest_->isChecked()) {
    result.append("Yearly Interest Rate = ");
    result.append(QString::number((double) calculator_->calculateInterestRate(), 'f', 2));
  }
  else if(radioNumPayments_->isChecked()) {
    result.append("Number of payments = ");
    result.append(QString::number((double) calculator_->calculateNumberPayments(), 'f', 2));
  }
  else if(radioBalance_->isChecked()) {
    result.append("Loan Balance = ");
    result.append(QString::number((double) calculator_->calculateLoanBalance(), 'f', 2));
  }
  // else this is impossible, one of them has to be selected

  //textEditCalcResults_->setText(QString::fromStdString(calculator_->toString()));
  textEditCalcResults_->setText(result);
}

void LoanCalcQtMainWindow::pressedButtonClearEntries()
{
  lineEditAmount_->clear();
  lineEditInitialPayment_->clear();
  lineEditLoanFeePercent_->clear();
  lineEditInterest_->clear();
  lineEditPayment_->clear();
  lineEditMonths_->clear();

  textEditCalcResults_->clear();
}

