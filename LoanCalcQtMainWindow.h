
#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QtGui>
#include "LoanCalculator.h"

class LoanCalcQtMainWindow : public QMainWindow
{
  Q_OBJECT

public:
  LoanCalcQtMainWindow(LoanCalculator *calculator);
  ~LoanCalcQtMainWindow();

public slots:
  // Radio Button SLOTs
  void pressedRadioMonthlyPayment();
  void pressedRadioAmount();
  void pressedRadioInterest();
  void pressedRadioNumPayments();
  void pressedRadioBalance();

  // Push Button SLOTs
  void pressedButtonCalculate();
  void pressedButtonClearEntries();

private:
  LoanCalcQtMainWindow(); // Cant initialize default version

  void getInputFields();
  void enableFields(bool enableAmount,
                    bool enableInitialPayment,
                    bool enableLoanFee,
                    bool enableInterest,
                    bool enablePayment,
                    bool enableMonths);

  QWidget *createCalcTypeOptions();
  QWidget *createInputFields();
  QWidget *createCalcResults();
  QWidget *createButtons();

  void deleteCalcTypeOptions();
  void deleteInputFields();
  void deleteCalcResults();
  void deleteButtons();

  LoanCalculator *calculator_;

  // Main window widgets
  QWidget *mainWidget_;
  QVBoxLayout *mainLayout_;

  // Calculation type fields
  QVBoxLayout *layoutVboxCalcType_;
  QGroupBox *groupBoxCalcType_;
  QRadioButton *radioMonthlyPayment_;
  QRadioButton *radioAmount_;
  QRadioButton *radioInterest_;
  QRadioButton *radioNumPayments_;
  QRadioButton *radioBalance_;

  // Input fields
  QGridLayout *layoutGridInputFields_;
  QGroupBox *groupBoxInputFields_;
  QLabel *labelAmount_;
  QLabel *labelInitialPayment_;
  QLabel *labelLoanFeePercent_;
  QLabel *labelInterest_;
  QLabel *labelPayment_;
  QLabel *labelMonths_;
  QLineEdit *lineEditAmount_;
  QLineEdit *lineEditInitialPayment_;
  QLineEdit *lineEditLoanFeePercent_;
  QLineEdit *lineEditInterest_;
  QLineEdit *lineEditPayment_;
  QLineEdit *lineEditMonths_;
  QDoubleValidator *doubleValidator_;
  QIntValidator *intValidator_;

  // Calulation Results
  QVBoxLayout *layoutVboxCalcResults_;
  QGroupBox *groupBoxCalcResults_;
  QTextEdit *textEditCalcResults_;

  // Buttons
  QHBoxLayout *layoutHboxButtons_;
  QGroupBox *groupBoxButtons_;
  QPushButton *buttonCalculate_;
  QPushButton *buttonClearEntries_;
};

#endif
