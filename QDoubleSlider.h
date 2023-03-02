/*=========================================================================

  Name:        QDoubleSlider.h

  Author:      David Borland, The Renaissance Computing Institute (RENCI)

  Copyright:   The Renaissance Computing Institute (RENCI)

  Description: Combines a QSlider and QDoubleSpinBox to enable a QSlider
               with floating-point values.

=========================================================================*/


#ifndef QDOUBLESLIDER_H
#define QDOUBLESLIDER_H


#include <QObject>


class QSlider;
class QDoubleSpinBox;


class QDoubleSlider : public QObject
{
    Q_OBJECT

public:
    QDoubleSlider(QSlider* slider, QDoubleSpinBox* spinBox, QObject* parent = 0);

    double value();

public slots:
    void setValue(double value);

    // XXX: Experimental 
    void setExponent(double exponent);

signals:
    void valueChanged(double value);
    void sliderReleased();

private slots:
    void setValueFromSlider(int value);
    void releaseSlider();
    void setValueFromSpinBox(double value);

private:
    QSlider* slider;
    QDoubleSpinBox* spinBox;

    // XXX: Experimental
    double exponent;
};


#endif