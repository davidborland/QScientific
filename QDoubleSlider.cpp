/*=========================================================================

  Name:        QDoubleSlider.cpp

  Author:      David Borland, The Renaissance Computing Institute (RENCI)

  Copyright:   The Renaissance Computing Institute (RENCI)

  Description: Combines a QSlider and QDoubleSpinBox to enable a QSlider
               with floating-point values.

=========================================================================*/


#include "QDoubleSlider.h"

#include <QSlider>
#include <QDoubleSpinBox>

#include <math.h>


QDoubleSlider::QDoubleSlider(QSlider* slider, QDoubleSpinBox* spinBox, QObject* parent)
    : QObject(parent), slider(slider), spinBox(spinBox)
{
    // Connect signals and slots
    connect(slider, SIGNAL(valueChanged(int)), this, SLOT(setValueFromSlider(int)));
    connect(slider, SIGNAL(sliderReleased()), this, SLOT(releaseSlider()));
    connect(spinBox, SIGNAL(valueChanged(double)), this, SLOT(setValueFromSpinBox(double)));

    // XXX: Set slider range to start at zero so we don't need to use slider->minimum() below?

    // XXX: Testing exponential
    exponent = 1.0;
}


double QDoubleSlider::value()
{
    return spinBox->value();
}


void QDoubleSlider::setValue(double value)
{
    spinBox->setValue(value);
    
    // This will force an update of the slider value
    setValueFromSpinBox(value);
}


void QDoubleSlider::setValueFromSlider(int value) 
{
    // Set the spin box value
    double fraction = (double)(value - slider->minimum()) / (double)(slider->maximum() - slider->minimum());


    // XXX: Testing exponential
    fraction = pow(fraction, exponent);

    
    double newValue = spinBox->minimum() + fraction * (spinBox->maximum() - spinBox->minimum());
        
    spinBox->setValue(newValue);

    // Emit the value as a signal
    emit valueChanged(newValue);
}

void QDoubleSlider::releaseSlider()
{
    // Just emit the appropriate signal
    emit sliderReleased();
}

void QDoubleSlider::setValueFromSpinBox(double value) 
{        
    // Set the slider value
    double fraction = (value - spinBox->minimum()) / (double)(spinBox->maximum() - spinBox->minimum());


    // XXX: Testing exponential
    fraction = pow(fraction, 1.0 / exponent);


    slider->blockSignals(true);
    slider->setValue(slider->minimum() + fraction * (slider->maximum() - slider->minimum()));
    slider->blockSignals(false);

    // Emit the value as a signal
    emit valueChanged(value);
}

void QDoubleSlider::setExponent(double exponent)
{
    this->exponent = exponent;

    // The value isn't actually changing, so block signals
    blockSignals(true);
    setValueFromSpinBox(spinBox->value());
    blockSignals(false);
}
