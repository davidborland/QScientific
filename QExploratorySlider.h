/*=========================================================================

  Name:        QExploratorySlider.h

  Author:      David Borland, The Renaissance Computing Institute (RENCI)

  Copyright:   The Renaissance Computing Institute (RENCI)

  Description: A widget that enables controlling the output of a slider
               via a combination of two power functions.

=========================================================================*/


#ifndef QEXPLORATORYSLIDER_H
#define QEXPLORATORYSLIDER_H


#include "QNonlinearSlider.h"


class QExploratorySlider : public QNonlinearSlider
{
    Q_OBJECT

public:
    QExploratorySlider(QWidget* parent = 0);

    double getExponent();
    double getPivotValue();

public slots:
    void setExponent(double e);
    void setPivotValue(double pv);

signals:
    void exponentChanged(double v);
    void pivotValueChanged(double pv);

protected:
    // Exponent for power function
    double exponent;

    // Pivot value for curve, normalized to [0, 1]
    double pivotValue;  

    // Positions of curve points, in normalized coordinates
    QPointF curvePoint1;
    QPointF curvePoint2;
    QPointF curvePoint3;

    // Size of drawn pivot value, in pixels
    double pivotRadius;

    // Interaction states
    enum QExploratorySliderAction {
        QExploratorySliderNoAction,
        QExploratorySliderMoveHandle,
        QExploratorySliderChangeExponent
    };
    QExploratorySliderAction action;

    // Internal methods
    virtual void paintEvent(QPaintEvent* event);

    void mousePressEvent(QMouseEvent* event);
    void mouseDoubleClickEvent(QMouseEvent* event);
    void mouseReleaseEvent(QMouseEvent* event);
    void mouseMoveEvent(QMouseEvent* event);
    
    double widgetXFromValue(double v) const;
    double valueFromWidgetX(double x) const;

    void buildCurve();
    double evaluateCurve(double x, QPointF p1, QPointF p2, double e) const;
    double evaluateCurveInverse(double y, QPointF p1, QPointF p2, double e) const;
};


#endif