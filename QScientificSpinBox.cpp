/*=========================================================================

  Name:        QScientificSpinBox.cpp

  Author:      David Borland, The Renaissance Computing Institute (RENCI)

  Copyright:   The Renaissance Computing Institute (RENCI)

  Description: A QDoubleSpinBox that can display in scientific notation.

=========================================================================*/


#include "QScientificSpinBox.h"

#include <limits>


QScientificSpinBox::QScientificSpinBox(QWidget* parent) 
    : QDoubleSpinBox(parent)
{
    // Set the precision to a reasonable value
    precision = 4;

    // Set the decimals to the largest possible value
    setDecimals(std::numeric_limits<int>::max());
}


void QScientificSpinBox::setPrecision(int precision)
{
    this->precision = precision;
}


QString QScientificSpinBox::textFromValue(double value) const
{
    QString str = locale().toString(value, 'e', precision);
    if (qAbs(value) >= 1000.0) {
        str.remove(locale().groupSeparator());
    }
    return str;
}
