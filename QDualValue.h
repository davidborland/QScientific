/*=========================================================================

  Name:        QDualValue.h

  Author:      David Borland, The Renaissance Computing Institute (RENCI)

  Copyright:   The Renaissance Computing Institute (RENCI)

  Description: A widget that returns two values based on vertical and
               horizontal mouse motion.

=========================================================================*/


#ifndef QDUALVALUE_H
#define QDUALVALUE_H


#include <QWidget>


class QDualValue : public QWidget
{
    Q_OBJECT

public:
    QDualValue(QWidget* parent = 0);

    double getValue1() const;
    double getValue2() const;

    void setValue1Minimum(double min);
    void setValue1Maximum(double max);
    void setValue1Range(double min, double max);

    void setValue2Minimum(double min);
    void setValue2Maximum(double max);
    void setValue2Range(double min, double max);

    void setMoveSeparately(bool separately);

public slots:
    void setValue1(double v);
    void setValue2(double v);
    void setValues(double v1, double v2);

signals:
    void value1Changed(double v);
    void releaseValue1();

    void value2Changed(double v);
    void releaseValue2();

    void valuesChanged(QPointF values);
    void releaseValues();

protected:
    // Values
    double value1;
    double value2;

    // Value 1 range
    double value1Minimum;
    double value1Maximum;

    // Value 2 range
    double value2Minimum;
    double value2Maximum;

    // Separate horizontal and vertical motion or not
    bool moveSeparately;

    // Position of handle, in normalized coordinates
    QPointF handle;
    
    // Rectangles for line controls, in pixel coordinates
    QRect rect1;
    QRect rect2;    

    // Handle radius and line width
    int handleRadius;
    int lineWidth;

    // Border from edge of widget
    int borderX;
    int borderY;

    // Mouse interaction variables
    QPoint oldMousePosition;
    QPointF oldHandlePosition;

    enum QDualValueAction {
        QDualValueNoAction,
        QDualValueMoveHandle,
        QDualValueMoveLine1,
        QDualValueMoveLine2
    };
    QDualValueAction action;

    // Internal methods
    virtual void mousePressEvent(QMouseEvent* event);
    virtual void mouseReleaseEvent(QMouseEvent* event);
    virtual void mouseMoveEvent(QMouseEvent* event);
    
    virtual void paintEvent(QPaintEvent* event);

    virtual void setWidgetFromValues();
    virtual void updateLines();

    virtual void setValue1FromWidget();
    virtual void setValue2FromWidget();
    virtual void setValuesFromWidget();

    virtual double value1FromWidget(double x) const;
    virtual double value2FromWidget(double y) const;
    
    virtual QPoint pixelsFromWidget(QPointF p) const;

    // Returns the width and height in pixels used for the slider area
    virtual int functionWidth() const;
    virtual int functionHeight() const;
};


#endif