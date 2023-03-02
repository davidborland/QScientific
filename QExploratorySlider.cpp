/*=========================================================================

  Name:        QExploratorySlider.h

  Author:      David Borland

  Copyright:   The Renaissance Computing Institute (RENCI)

  Description: A widget that enables controlling the output of a slider
               via a combination of two power functions.

=========================================================================*/


#include "QExploratorySlider.h"

#include <QtCore/qmath.h>
#include <QMouseEvent>
#include <QPainter>


QExploratorySlider::QExploratorySlider(QWidget* parent)
    : QNonlinearSlider(parent)
{
    // Default to linear function
    exponent = 1.0;

    // Default pivot value
    pivotValue = 0.5;
   
    // Build the curve
    buildCurve();

    // No interaction to start with
    action = QExploratorySliderNoAction;

    // Appearance
    pivotRadius = 1.5;
}


double QExploratorySlider::getExponent()
{
    return exponent;
}


void QExploratorySlider::setExponent(double e)
{
    if (e == exponent) {
        return;
    }

    exponent = e;

    // Build the curve
    buildCurve();

    // Update the handle
    setHandleFromValue();

    // Emit the exponent as a signal
    emit exponentChanged(exponent);

    // Repaint
    update();
}


double QExploratorySlider::getPivotValue()
{
    return pivotValue;
}


void QExploratorySlider::setPivotValue(double pv)
{
    if (pv == pivotValue) {
        return;
    }

    pivotValue = pv;

    // Build the curve
    buildCurve();

    // Update the handle
    setHandleFromValue();

    // Emit the pivot value as a signal
    emit pivotValueChanged(pivotValue);
    
    // Repaint
    update();
}


void QExploratorySlider::mousePressEvent(QMouseEvent* event)
{
    // Only care about left-button presses
    if (event->button() != Qt::LeftButton) {
        event->ignore();

        return;
    }

    event->accept();

    // Get the event position
    QPoint p = event->pos();

    // Map to range
    p.setX(qBound(0, p.x(), width()));
    p.setY(qBound(0, p.y(), height()));

    // Intersect with controls
    action = QExploratorySliderNoAction;

    QPointF d = p - pixelsFromWidget(handle);

    if (qSqrt(d.x() * d.x() + d.y() * d.y()) <= handleRadius) {
        action = QExploratorySliderMoveHandle;

        oldHandlePosition = handle;
    }
    else {
        action = QExploratorySliderChangeExponent;
    }

    // Save mouse position
    oldMousePosition = p;
}


void QExploratorySlider::mouseDoubleClickEvent(QMouseEvent* event)
{
    // Only care about left- and right-button presses
    if (event->button() != Qt::LeftButton &&
        event->button() != Qt::RightButton) {
        event->ignore();

        return;
    }

    event->accept();

    switch (event->button()) {

        case Qt::LeftButton: 

            // Set the pivot value
            setPivotValue(widgetYFromValue(value));

            break;

        case Qt::RightButton:

            // Reset exponent and pivot
            setExponent(1.0);
            setPivotValue(0.5);

            break;
    }
}


void QExploratorySlider::mouseReleaseEvent(QMouseEvent* event)
{
    // Check action variable
    if (action == QExploratorySliderNoAction) {
        event->ignore();

        return;
    }

    event->accept();

    if (action == QExploratorySliderMoveHandle) {
        emit sliderReleased();
    }

    // Clear action variable
    action = QExploratorySliderNoAction;
}


void QExploratorySlider::mouseMoveEvent(QMouseEvent* event)
{
    // Check action variable
    if (action == QExploratorySliderNoAction) {
        event->ignore();

        return;
    }

    event->accept();

    // Get the delta between the last event and here
    QPoint delta = event->pos() - oldMousePosition;
    delta.setY(-delta.y());

    // Save the mouse position
    oldMousePosition = event->pos();

    switch (action) {

        case QExploratorySliderMoveHandle:   
            // Move handle
            handle.setX(qBound(0.0, oldHandlePosition.x() + (double)delta.x() / width(), 1.0));

            // Update value
            setValueFromHandle();

            break;

        case QExploratorySliderChangeExponent:
            if (delta.y() < 0) {
                // Increase exponent
                setExponent(exponent * 1.1);
            }
            else if (delta.y() > 0) {
                // Decrease exponent
                setExponent(exponent / 1.1);
            }

            break;
    }

    // Save the widget position
    oldHandlePosition = handle;
    
    // Repaint
    update();
}


void QExploratorySlider::paintEvent(QPaintEvent* event)
{
    QPainter painter(this);


    // Positions and sizes may need to change
//    setHandleFromValue();


    // Draw background cross
    painter.setRenderHint(QPainter::Antialiasing, false);

    painter.setPen(palette().midlight());

    painter.drawLine(pixelsFromWidget(QPointF(0.5, 0.0)), pixelsFromWidget(QPointF(0.5, 1.0)));
    painter.drawLine(pixelsFromWidget(QPointF(0.0, 0.5)), pixelsFromWidget(QPointF(1.0, 0.5)));

    
    // Draw border
    painter.setRenderHint(QPainter::Antialiasing, false);

    painter.setPen(palette().mid());
    painter.setBrush(Qt::NoBrush);    

    painter.drawRect(borderX - 1, borderY - 1, functionWidth() + 1, functionHeight() + 1);


    // Draw function
//    painter.setPen(Qt::black);
    painter.setRenderHint(QPainter::Antialiasing);

    QPointF previous(borderX, functionHeight() + borderY);
    for (int i = borderX + 1; i <= borderX + functionWidth(); i++) {
        double x = (double)(i - borderX) / functionWidth();
        double y = widgetYFromValue(valueFromWidgetX(x));

        QPointF p = pixelsFromWidget(QPointF(x, y));

        painter.drawLine(previous, p);

        previous = p;
    }
    

    // Draw pivot
    painter.setBrush(palette().mid());

    painter.drawEllipse(pixelsFromWidget(QPointF(widgetXFromValue(valueFromWidgetY(pivotValue)), pivotValue)), pivotRadius, pivotRadius);


    // Draw value
    QColor color = palette().window().color();
    painter.setBrush(color);

    painter.drawEllipse(pixelsFromWidget(QPointF(handle.x(), widgetYFromValue(value))), valueRadius, valueRadius);


    // Draw handle
    color.setAlphaF(0.5);
    painter.setBrush(color);
    painter.setPen(Qt::black);

    painter.drawEllipse(pixelsFromWidget(handle), handleRadius, handleRadius);


    // Draw text
//    painter.drawText(rect(), Qt::AlignTop, locale().toString(value, 'e', 4));
}


double QExploratorySlider::widgetXFromValue(double v) const
{
    // Find the x position
    double x;
    double y = widgetYFromValue(v);

    if (exponent < 1.0) {
        double e = 1.0 / exponent;

        if (y <= curvePoint2.y()) {
            x = evaluateCurveInverse(y, curvePoint1, curvePoint2, 1.0 / e);
        }
        else {
            x = evaluateCurveInverse(y, curvePoint3, curvePoint2, 1.0 / e);
        }
    }
    else {
        double e = exponent;

        if (y <= curvePoint2.y()) {
            x = evaluateCurveInverse(y, curvePoint2, curvePoint1, 1.0 / e);
        }
        else {
            x = evaluateCurveInverse(y, curvePoint2, curvePoint3, 1.0 / e);
        }
    }

    return x;
}

double QExploratorySlider::valueFromWidgetX(double x) const
{
    // Map from x to y
    double y;

    if (exponent < 1.0) {
        double e = 1.0 / exponent;

        if (x <= curvePoint2.x()) {
            y = evaluateCurve(x, curvePoint1, curvePoint2, e);
        }
        else {
            y = evaluateCurve(x, curvePoint3, curvePoint2, e);
        }
    }
    else {
        double e = exponent;

        if (x <= curvePoint2.x()) {
            y = evaluateCurve(x, curvePoint2, curvePoint1, e);
        }
        else {
            y = evaluateCurve(x, curvePoint2, curvePoint3, e);
        }
    }

    // Map to value space
    return valueFromWidgetY(y);
}


void QExploratorySlider::buildCurve()
{
    // XXX: Explain better

    // When exponent is greater than 1.0, we use two curves moving away from the pivot.
    // When exponent is less than 1.0, we invert the exponent, but use two curves moving
    // away from 0 and 1.  This enables flattening the values either at either side of the 
    // pivot, or from 0 or 1 to the pivot.  When exponent is less than 1.0, we also use
    // 1.0 - pivot as the pivot value to make the curve longer.

    bool invert = exponent < 1.0;

    // The exponent and pivot value
    double e = invert ? 1.0 / exponent : exponent;
    double pv = invert ? 1.0 - pivotValue : pivotValue;

    // Find the smallest distance from the pivot value to 0 or 1
    double pivotDistance = qMin(pv, 1.0 - pv);

    // Find the height of the curves
    double curveHeight = 1.0 - pivotDistance;

    // Find the x-intercept with the top or bottom
    double intercept = invert ? 1.0 - qPow(1.0 - pivotDistance / curveHeight, 1.0 / e) 
                              : qPow(pivotDistance / curveHeight, 1.0 / e);

    // Find the x pivot position
    double pivotX = pv <= 0.5 ? intercept : 1.0;
    pivotX /= (1.0 + intercept);

    // Find the width of the curves, based on the x pivot position
    double curveWidth = qMax(pivotX, 1.0 - pivotX);

    // Find the points of the curve
    curvePoint1 = QPointF(pivotX - curveWidth, pv - curveHeight);
    curvePoint2 = QPointF(pivotX, pv);
    curvePoint3 = QPointF(pivotX + curveWidth, pv + curveHeight);
}

double QExploratorySlider::evaluateCurve(double x, QPointF p1, QPointF p2, double e) const
{
    // Get the signed width and height
    double w = p2.x() - p1.x();
    double h = p2.y() - p1.y();

    // Find the distance in x from point 1
    double d = abs((p1.x() - x) / w);

    // Compute the power function
    double f = qPow(d, e);

    // Map to y
    return p1.y() + f * h; 
}

double QExploratorySlider::evaluateCurveInverse(double y, QPointF p1, QPointF p2, double e) const
{
    // Get the signed width and height
    double w = p2.x() - p1.x();
    double h = p2.y() - p1.y();

    // Find the distance in y from point 1
    double d = abs((p1.y() - y) / h);

    // Compute the power function
    double f = qPow(d, e);

    // Map to x
    return p1.x() + f * w; 
}