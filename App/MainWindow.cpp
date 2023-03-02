/*=========================================================================

  Name:        MainWindow.cpp

  Author:      David Borland

  Copyright:   The Renaissance Computing Institute (RENCI)

  Description: The main window for the QScientificTest application

=========================================================================*/


#include "MainWindow.h"

#include "QDoubleSlider.h"


MainWindow::MainWindow(QWidget* parent) : QMainWindow(parent) {
    // Create the GUI from the Qt Designer file
    setupUi(this);
    
    
    // Set up double sliders
    doubleSlider1 = new QDoubleSlider(slider1, spinBox1, this);
    doubleSlider2 = new QDoubleSlider(slider2, spinBox2, this);

    
    // Data range
    double min = -1000.0;
    double max = 1000.0;

    // Control QDoubleSlider via the associated spin box
    spinBox1->setRange(min, max);
    spinBox2->setRange(min, max);

    // Set other ranges
    powerSlider1->setRange(min, max);
    powerSlider2->setRange(min, max);
    exploratorySlider1->setRange(min, max);
    exploratorySlider2->setRange(min, max);
    dualValue->setValue1Range(min, max);
    dualValue->setValue2Range(min, max);


    // Make connections
    connect(doubleSlider1, SIGNAL(valueChanged(double)), powerSlider1, SLOT(setValue(double)));
    connect(powerSlider1, SIGNAL(valueChanged(double)), exploratorySlider1, SLOT(setValue(double)));
    connect(exploratorySlider1, SIGNAL(valueChanged(double)), dualValue, SLOT(setValue1(double)));
    connect(dualValue, SIGNAL(value1Changed(double)), doubleSlider1, SLOT(setValue(double)));
    
    connect(doubleSlider2, SIGNAL(valueChanged(double)), powerSlider2, SLOT(setValue(double)));
    connect(powerSlider2, SIGNAL(valueChanged(double)), exploratorySlider2, SLOT(setValue(double)));
    connect(exploratorySlider2, SIGNAL(valueChanged(double)), dualValue, SLOT(setValue2(double)));
    connect(dualValue, SIGNAL(value2Changed(double)), doubleSlider2, SLOT(setValue(double)));


    // Set the value
    doubleSlider1->setValue(0.0);
    doubleSlider2->setValue(0.0);
}

MainWindow::~MainWindow() {
}


///////////////////////////////////////////////////////////////////////////
// Respond to menu events

void MainWindow::on_actionExit_triggered() {
    qApp->exit();
}


///////////////////////////////////////////////////////////////////////////
// Respond to widget events

void MainWindow::on_checkBox_toggled(bool checked) {
    if (checked) {
        // Connect curves
        connect(powerSlider1, SIGNAL(exponentChanged(double)), powerSlider2, SLOT(setExponent(double)));
        connect(powerSlider2, SIGNAL(exponentChanged(double)), powerSlider1, SLOT(setExponent(double)));

        connect(exploratorySlider1, SIGNAL(exponentChanged(double)), exploratorySlider2, SLOT(setExponent(double)));
        connect(exploratorySlider2, SIGNAL(exponentChanged(double)), exploratorySlider1, SLOT(setExponent(double)));
  
        connect(exploratorySlider1, SIGNAL(pivotValueChanged(double)), exploratorySlider2, SLOT(setPivotValue(double)));
        connect(exploratorySlider2, SIGNAL(pivotValueChanged(double)), exploratorySlider1, SLOT(setPivotValue(double)));


        // Set curve 2 equal to curve 1
        powerSlider2->setExponent(powerSlider1->getExponent());
        
        exploratorySlider2->setExponent(exploratorySlider1->getExponent());
        exploratorySlider2->setPivotValue(exploratorySlider1->getPivotValue());
    }
    else {
        // Disconnect curves
        disconnect(powerSlider1, SIGNAL(exponentChanged(double)), powerSlider2, SLOT(setExponent(double)));
        disconnect(powerSlider2, SIGNAL(exponentChanged(double)), powerSlider1, SLOT(setExponent(double)));

        disconnect(exploratorySlider1, SIGNAL(exponentChanged(double)), exploratorySlider2, SLOT(setExponent(double)));
        disconnect(exploratorySlider2, SIGNAL(exponentChanged(double)), exploratorySlider1, SLOT(setExponent(double)));
  
        disconnect(exploratorySlider1, SIGNAL(pivotValueChanged(double)), exploratorySlider2, SLOT(setPivotValue(double)));
        disconnect(exploratorySlider2, SIGNAL(pivotValueChanged(double)), exploratorySlider1, SLOT(setPivotValue(double)));
    }
}