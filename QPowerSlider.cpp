/*=========================================================================

  Name:        QPowerSlider.h

  Author:      David Borland, The Renaissance Computing Institute (RENCI)

  Copyright:   The Renaissance Computing Institute (RENCI)

  Description: A widget that enables controlling the output of a slider
               via a power function.

=========================================================================*/


#include "QPowerSlider.h"

#include <QtCore/qmath.h>
#include <QMouseEvent>


QPowerSlider::QPowerSlider(QWidget* parent)
    : QNonlinearSlider(parent)
{
    // Default to linear function
    exponent = 1.0;

    // No interaction to start with
    action = QPowerSliderNoAction;
}


double QPowerSlider::getExponent()
{
    return exponent;
}


void QPowerSlider::setExponent(double e)
{
    if (e == exponent) {
        return;
    }

    exponent = e;

    // Update the handle
    setHandleFromValue();

    // Emit the exponent as a signal
    emit exponentChanged(exponent);

    // Repaint
    update();
}


void QPowerSlider::mousePressEvent(QMouseEvent* event)
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

    // Intersect with control
    action = QPowerSliderNoAction;

    QPointF d = p - pixelsFromWidget(handle);

    if (qSqrt(d.x() * d.x() + d.y() * d.y()) <= handleRadius) {
        action = QPowerSliderMoveHandle;

        oldHandlePosition = handle;
    }
    else {
        action = QPowerSliderChangeExponent;
    }

    // Save mouse position
    oldMousePosition = p;
}


void QPowerSlider::mouseReleaseEvent(QMouseEvent* event)
{
    // Check action variable
    if (action == QPowerSliderNoAction) {
        event->ignore();

        return;
    }

    event->accept();

    if (action == QPowerSliderMoveHandle) {
        emit sliderReleased();
    }

    // Clear action variable
    action = QPowerSliderNoAction;
}


void QPowerSlider::mouseMoveEvent(QMouseEvent* event)
{
    // Check action variable
    if (action == QPowerSliderNoAction) {
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

        case QPowerSliderMoveHandle:     
            // Move handle
            handle.setX(qBound(0.0, oldHandlePosition.x() + (double)delta.x() / width(), 1.0));

            // Update value
            setValueFromHandle();

            break;

        case QPowerSliderChangeExponent:         
            if (delta.y() < 0) {
                // Increase exponent
                setExponent(exponent * 1.1);
            }
            else {
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


double QPowerSlider::widgetXFromValue(double v) const
{
    double y = widgetYFromValue(v);

    if (exponent < 1.0) {
        // Instead of using an exponent < 1.0, flip the curve to flatten out curve horizontally near 1.0
        return 1.0 - qPow(1.0 - y, exponent);   
    }
    else {
        double e = 1.0 / exponent;

        return qPow(y, e);
    }
}


double QPowerSlider::valueFromWidgetX(double x) const
{
    if (exponent < 1.0) {
        // Instead of using an exponent < 1.0, flip the curve to flatten out curve horizontally near 1.0
        double e = 1.0 / exponent;

        return minimum + (1.0 - qPow((1.0 - x), e)) * (maximum - minimum);
    }
    else {
        return minimum + qPow(x, exponent) * (maximum - minimum);
    }
}