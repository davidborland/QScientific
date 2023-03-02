/*=========================================================================

  Name:        QNonlinearSlider.h

  Author:      David Borland, The Renaissance Computing Institute (RENCI)

  Copyright:   The Renaissance Computing Institute (RENCI)

  Description: Abstract base class for sliders that use a nonlinear function
               to map slider position to data value.

=========================================================================*/


#ifndef QNONLINEARSLIDER_H
#define QNONLINEARSLIDER_H


#include <QWidget>


class QNonlinearSlider : public QWidget
{
    Q_OBJECT

public:
    QNonlinearSlider(QWidget* parent = 0);

    double getValue() const;

    void setMinimum(double min);
    void setMaximum(double max);
    void setRange(double min, double max);

    virtual QSize sizeHint() const;
    virtual QSize minimumSizeHint() const;

public slots:
    void setValue(double v);

signals:
    void valueChanged(double v);
    void sliderReleased();

protected:
    // Value
    double value;

    // Range
    double minimum;
    double maximum;

    // Position of slider handle, in normalized coordinates
    QPointF handle;    

    // Size of drawn elements, in pixels
    int handleRadius;
    int valueRadius;

    // Border from edge of widget
    int borderX;
    int borderY;

    // Mouse interaction variables
    QPoint oldMousePosition;
    QPointF oldHandlePosition;

    // Internal methods    
    virtual void paintEvent(QPaintEvent* event);

    virtual void setHandleFromValue();
    virtual void setValueFromHandle();

    virtual double widgetXFromValue(double v) const = 0;
    virtual double widgetYFromValue(double v) const;

    virtual double valueFromWidgetX(double x) const = 0;
    virtual double valueFromWidgetY(double y) const;

    virtual QPointF pixelsFromWidget(QPointF p) const;

    // Returns the width and height in pixels used for the function
    virtual int functionWidth() const;
    virtual int functionHeight() const;
};


#endif