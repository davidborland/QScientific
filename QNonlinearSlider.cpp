/*=========================================================================

  Name:        QNonlinearSlider.h

  Author:      David Borland, The Renaissance Computing Institute (RENCI)

  Copyright:   The Renaissance Computing Institute (RENCI)

  Description: A widget that returns two values based on vertical and
               horizontal mouse motion.

=========================================================================*/


#include "QNonlinearSlider.h"

#include <QPainter>


QNonlinearSlider::QNonlinearSlider(QWidget* parent)
    : QWidget(parent)
{
    // Set default range
    minimum = 0.0;
    maximum = 1.0;

    // Set default value
    value = 0.5;

    // Set handle to center
    handle.setX(0.5);
    handle.setY(0.5);
    
    // Appearance
    handleRadius = 7;
    valueRadius = handleRadius / 2;

    borderX = handleRadius + 1;
    borderY = valueRadius + 1;
}


double QNonlinearSlider::getValue() const
{
    return value;
}


void QNonlinearSlider::setValue(double v) 
{
    v = qBound(minimum, v, maximum);

    if (v == value) {
        return;
    }

    value = v;

    setHandleFromValue();

    // Emit the value as a signal
    emit valueChanged(value);

    // Repaint
    update();
}


void QNonlinearSlider::setMinimum(double min)
{
    // Ensure a valid range
    setRange(min, qMax(maximum, min));
}

void QNonlinearSlider::setMaximum(double max)
{
    // Ensure a valid range
    setRange(qMin(minimum, max), max);
}

void QNonlinearSlider::setRange(double min, double max)
{
    int oldMin = minimum;
    int oldMax = maximum;

    minimum = qMin(min, max);
    maximum = qMax(min, max);

    if (oldMin != minimum || oldMax != maximum) {
        // Ensure valid value
        setValue(value);
    }
}


QSize QNonlinearSlider::sizeHint() const
{
    int h = handleRadius * 6;

    return QSize(h*3, h);
}

QSize QNonlinearSlider::minimumSizeHint() const
{
    int h = handleRadius * 4;

    return QSize(h*3, h);
}


void QNonlinearSlider::paintEvent(QPaintEvent* event)
{
    QPainter painter(this);


    // Positions and sizes may need to change
//    setHandleFromValue();

    
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


void QNonlinearSlider::setHandleFromValue()
{
    handle.setX(widgetXFromValue(value));
    handle.setY(0.5);
}

void QNonlinearSlider::setValueFromHandle()
{
    double v = valueFromWidgetX(handle.x());

    if (v == value) {
        return;
    }

    value = v;    

    // Emit the value as a signal
    emit valueChanged(value);
}


double QNonlinearSlider::widgetYFromValue(double v) const
{
    return (v - minimum) / (maximum - minimum);
}

double QNonlinearSlider::valueFromWidgetY(double y) const
{
    return minimum + y * (maximum - minimum);
}

QPointF QNonlinearSlider::pixelsFromWidget(QPointF p) const
{
    return QPointF(borderX + p.x() * functionWidth(), height() - borderY - p.y() * functionHeight());
}


int QNonlinearSlider::functionWidth() const
{
    return width() - borderX * 2;
}

int QNonlinearSlider::functionHeight() const
{
    return height() - borderY * 2;
}