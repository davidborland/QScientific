/*=========================================================================

  Name:        QPowerSlider.h

  Author:      David Borland, The Renaissance Computing Institute (RENCI)

  Copyright:   The Renaissance Computing Institute (RENCI)

  Description: A widget that enables controlling the output of a slider
               via a power function.

=========================================================================*/


#ifndef QPOWERSLIDER_H
#define QPOWERSLIDER_H


#include "QNonlinearSlider.h"


class QPowerSlider : public QNonlinearSlider
{
    Q_OBJECT

public:
    QPowerSlider(QWidget* parent = 0);

    double getExponent();

public slots:
    void setExponent(double e);

signals:
    void exponentChanged(double e);

protected:
    // Exponent for power function
    double exponent;

    // Interaction states
    enum QPowerSliderAction {
        QPowerSliderNoAction,
        QPowerSliderMoveHandle,
        QPowerSliderChangeExponent
    };
    QPowerSliderAction action;

    // Internal methods
    void mousePressEvent(QMouseEvent* event);
    void mouseReleaseEvent(QMouseEvent* event);
    void mouseMoveEvent(QMouseEvent* event);

    double widgetXFromValue(double v) const;
    double valueFromWidgetX(double x) const;
};


#endif