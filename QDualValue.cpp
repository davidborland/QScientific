/*=========================================================================

  Name:        QDualValue.h

  Author:      David Borland, The Renaissance Computing Institute (RENCI)

  Copyright:   The Renaissance Computing Institute (RENCI)

  Description: A widget that returns two values based on vertical and
               horizontal mouse motion.

=========================================================================*/


#include "QDualValue.h"

#include <QtCore/qmath.h>
#include <QPainter>
#include <QMouseEvent>


QDualValue::QDualValue(QWidget* parent)
    : QWidget(parent)
{
    // Set default ranges
    value1Minimum = 0.0;
    value1Maximum = 1.0;

    value2Minimum = 0.0;
    value2Maximum = 1.0;

    // Set default value
    value1 = 0.5;
    value2 = 0.5;

    moveSeparately = false;

    // Appearance
    handleRadius = 7;
    lineWidth = handleRadius;

    borderX = handleRadius + 1;
    borderY = borderX;

    // Set handle to center
    handle.setX(0.5);
    handle.setY(0.5);

    // No interaction to start with
    action = QDualValueNoAction;
}


double QDualValue::getValue1() const
{
    return value1;
}

double QDualValue::getValue2() const
{
    return value2;
}


void QDualValue::setValue1(double v) 
{
    // Set the value
    v = qBound(value1Minimum, v, value1Maximum);

    if (v == value1) {
        return;
    }

    value1 = v;

    setWidgetFromValues();

    // Emit the value as a signal
    emit value1Changed(value1);

    // Repaint
    update();
}

void QDualValue::setValue2(double v) 
{
    // Set the value
    v = qBound(value2Minimum, v, value2Maximum);

    if (v == value2) {
        return;
    }

    value2 = v;

    setWidgetFromValues();

    // Emit the value as a signal
    emit value2Changed(value2);

    // Repaint
    update();
}

void QDualValue::setValues(double v1, double v2)
{
    // Set the values
    v1 = qBound(value1Minimum, v1, value1Maximum);
    v2 = qBound(value2Minimum, v2, value2Maximum);

    if (v1 == value1 && v2 == value2) {
        return;
    }
    else if (v1 != value1 && v2 != value2) {
        value1 = v1;
        value2 = v2;

        setWidgetFromValues();

        // Emit the values as a signal
        emit valuesChanged(QPointF(value1, value2));

        emit value1Changed(value1);
        emit value2Changed(value2);
    }
    else if (v1 != value1) {
        value1 = v1;
    
        setWidgetFromValues();

        // Emit the value as a signal
        emit value1Changed(value1);
    }
    else if (v2 != value2) {
        value2 = v2;
      
        setWidgetFromValues();

        // Emit the value as a signal
        emit value2Changed(value2);
    }
            
    // Repaint
    update();
}


void QDualValue::setValue1Minimum(double min)
{
    // Ensure a valid range
    setValue1Range(min, qMax(value1Maximum, min));
}

void QDualValue::setValue1Maximum(double max)
{
    // Ensure a valid range
    setValue1Range(qMin(value1Minimum, max), max);
}

void QDualValue::setValue1Range(double min, double max)
{
    int oldMin = value1Minimum;
    int oldMax = value1Maximum;

    value1Minimum = qMin(min, max);
    value1Maximum = qMax(min, max);

    if (oldMin != value1Minimum || oldMax != value1Maximum) {
        // Ensure valid value
        setValue1(value1);
    }
}


void QDualValue::setValue2Minimum(double min)
{
    // Ensure a valid range
    setValue2Range(min, qMax(value2Maximum, min));
}

void QDualValue::setValue2Maximum(double max)
{
    // Ensure a valid range
    setValue2Range(qMin(value2Minimum, max), max);
}

void QDualValue::setValue2Range(double min, double max)
{
    int oldMin = value2Minimum;
    int oldMax = value2Maximum;

    value2Minimum = qMin(min, max);
    value2Maximum = qMax(min, max);

    if (oldMin != value2Minimum || oldMax != value2Maximum) {
        // Ensure valid value
        setValue2(value2);
    }
}


void QDualValue::setMoveSeparately(bool separately)
{
    moveSeparately = separately;
}


void QDualValue::mousePressEvent(QMouseEvent* event)
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
    action = QDualValueNoAction;

    QPointF d = p - pixelsFromWidget(handle);

    if (qSqrt(d.x() * d.x() + d.y() * d.y()) <= handleRadius) {
        action = QDualValueMoveHandle;
    }
    else if (rect1.contains(p)) {
        action = QDualValueMoveLine1;
    }
    else if (rect2.contains(p)) {
        action = QDualValueMoveLine2;
    }

    // Save handle position
    oldHandlePosition = handle;

    // Save mouse position
    oldMousePosition = p;
}


void QDualValue::mouseReleaseEvent(QMouseEvent* event)
{
    // Check action variable
    if (action == QDualValueNoAction) {
        event->ignore();

        return;
    }

    event->accept();

    switch (action) {

        case QDualValueMoveHandle:
            emit releaseValues();
            break;

        case QDualValueMoveLine1:
            emit releaseValue1();            
            break;
            
        case QDualValueMoveLine2:
            emit releaseValue2();
            break;
    }

    // Clear action variable
    action = QDualValueNoAction;
}


void QDualValue::mouseMoveEvent(QMouseEvent* event)
{
    // Check action variable
    if (action == QDualValueNoAction) {
        event->ignore();

        return;
    }

    event->accept();

    // Get the delta between the position when the mouse was first pressed and here
    QPoint delta = event->pos() - oldMousePosition;
    delta.setY(-delta.y());    
    
    // Save the mouse position
    oldMousePosition = event->pos();

    // Get new x and y
    double x = qBound(0.0, oldHandlePosition.x() + (double)delta.x() / width(), 1.0);
    double y = qBound(0.0, oldHandlePosition.y() + (double)delta.y() / height(), 1.0);
     
    switch (action) {

        case QDualValueMoveLine1:
            // Just updated horizontal value
            handle.setX(x);

            // Update value1
            setValue1FromWidget();

            break;

        case QDualValueMoveLine2:
            // Just update vertical value
            handle.setY(y);

            // Update value2
            setValue2FromWidget();

            break;

        case QDualValueMoveHandle:     
            // Update horizontal and vertical values
            if (moveSeparately) {
                if (abs(delta.x()) > abs(delta.y())) {
                    // Just updated horizontal position
                    handle.setX(x);

                    // Update values
                    setValuesFromWidget();
                }
                else if (abs(delta.y()) > abs(delta.x())) {
                    // Just update vertical position
                    handle.setY(y);
                    
                    // Update values
                    setValuesFromWidget();
                }
            }
            else {
                // Update both
                handle.setX(x);
                handle.setY(y);                    
                
                // Update values
                setValuesFromWidget();
            }

            break;
    }
        
    // Save the widget position
    oldHandlePosition = handle;
    
    // Repaint
    update();
}


void QDualValue::paintEvent(QPaintEvent* event)
{
    QPainter painter(this);


    // Positions and sizes may need to change
//    setWidgetFromValues();
    updateLines();
        

    // Draw border
    painter.setRenderHint(QPainter::Antialiasing, false);

    painter.setPen(palette().mid());
    painter.setBrush(Qt::NoBrush);    

    painter.drawRect(borderX, borderY, functionWidth(), functionHeight());


    // Draw rectangles
    QColor penColor = palette().mid();
    penColor = QColor(penColor.red() * 0.75, penColor.green() * 0.75, penColor.blue() * 0.75);

    QColor brushColor = palette().window().color();
    brushColor.setAlphaF(0.5);

//    painter.setPen(Qt::black);
    painter.setPen(penColor);
    painter.setBrush(brushColor);

    painter.drawRect(rect1);
    painter.drawRect(rect2);
    
    
    // Draw circle
    painter.setRenderHint(QPainter::Antialiasing);

    painter.setPen(Qt::black);
    painter.setBrush(palette().window());

    painter.drawEllipse(pixelsFromWidget(handle), handleRadius, handleRadius);


    // Draw text
//    painter.drawText(rect(), Qt::AlignCenter, locale().toString(value1, 'e', 4));
}


void QDualValue::setWidgetFromValues()
{
    handle.setX((value1 - value1Minimum) / (value1Maximum - value1Minimum));
    handle.setY((value2 - value2Minimum) / (value2Maximum - value2Minimum));

    updateLines();
}

void QDualValue::updateLines()
{
    QPointF h = pixelsFromWidget(handle);

    rect1.setRect(h.x() - lineWidth / 2 - 1, borderY, lineWidth, functionHeight());
    rect2.setRect(borderX, h.y() - lineWidth / 2 - 1, functionWidth(), lineWidth);
}


void QDualValue::setValue1FromWidget()
{
    double v = value1FromWidget(handle.x());

    if (v == value1) {
        return;
    }
    
    value1 = v;

    // Emit the value as a signal
    emit value1Changed(value1);
}

void QDualValue::setValue2FromWidget()
{
    double v = value2FromWidget(handle.y());

    if (v == value2) {
        return;
    }
    
    value2 = v;

    // Emit the value as a signal
    emit value2Changed(value2);
}

void QDualValue::setValuesFromWidget()
{
    // Set the values
    double v1 = value1FromWidget(handle.x());
    double v2 = value2FromWidget(handle.y());

    if (v1 == value1 && v2 == value2) {
        return;
    }
    else if (v1 != value1 && v2 != value2) {
        value1 = v1;
        value2 = v2;
        
        // Emit the values as a signal
        emit valuesChanged(QPointF(value1, value2));

        emit value1Changed(value1);
        emit value2Changed(value2);
    }
    else if (v1 != value1) {
        value1 = v1;
    
        // Emit the value as a signal
        emit value1Changed(value1);
    }
    else if (v2 != value2) {
        value2 = v2;
      
        // Emit the value as a signal
        emit value2Changed(value2);
    }
}



double QDualValue::value1FromWidget(double x) const
{
    return value1Minimum + x * (value1Maximum - value1Minimum);
}

double QDualValue::value2FromWidget(double y) const
{
    return value2Minimum + y * (value2Maximum - value2Minimum);
}


QPoint QDualValue::pixelsFromWidget(QPointF p) const
{
    return QPoint(borderX + p.x() * functionWidth(), height() - borderY - p.y() * functionHeight());
}


int QDualValue::functionWidth() const
{
    return width() - borderX * 2;
}

int QDualValue::functionHeight() const
{
    return height() - borderY * 2;
}