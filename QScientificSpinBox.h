/*=========================================================================

  Name:        QScientificSpinBox.h

  Author:      David Borland, The Renaissance Computing Institute (RENCI)

  Copyright:   The Renaissance Computing Institute (RENCI)

  Description: A QDoubleSpinBox that can display in scientific notation.

=========================================================================*/


#ifndef QSCIENTIFICSPINBOX_H
#define QSCIENTIFICSPINBOX_H

#include <QDoubleSpinBox>

class QScientificSpinBox : public QDoubleSpinBox
{
    Q_OBJECT

public:
    QScientificSpinBox(QWidget *parent = 0);

    // Set the number of digits after the decimal point of the displayed value.  
    // This is different from the decimals member in QDoubleSpinBox, which we are 
    // using to set the number of decimals used to store the internal value.  
    // The reason for this is that it is basically impossible to change the internal 
    // rounding behavior of QDoubleSpinBox without completely rewriting it, due to 
    // the inaccesiblity of QAbstractSpinBoxPrivate and some functions not being virtual.
    void setPrecision(int precision);

    // Reimplement textFromValue()
    virtual QString textFromValue(double value) const;

protected:
    // Number of digits displayed after the decimal point
    int precision;
};


#endif