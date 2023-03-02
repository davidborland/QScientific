/*=========================================================================

  Name:        MainWindow.h

  Author:      David Borland

  Copyright:   The Renaissance Computing Institute (RENCI)

  Description: The main window for the QScientificTest application

=========================================================================*/


#ifndef MAINWINDOW_H
#define MAINWINDOW_H


#include <QMainWindow>

#include "ui_MainWindow.h"

class QDoubleSlider;


class MainWindow : public QMainWindow, private Ui_MainWindow {
    Q_OBJECT

public:
    // Constructor/destructor
    MainWindow(QWidget* parent = NULL);
    virtual ~MainWindow();

public slots:
    // Use Qt's auto-connect magic to tie GUI widgets to slots,
    // removing the need to call connect() explicitly.
    // Names of the methods must follow the naming convention
    // on_<widget name>_<signal name>(<signal parameters>).

    // Menu events
    virtual void on_actionExit_triggered();

    // Widget events
    virtual void on_checkBox_toggled(bool checked);

protected:
    // Double sliders to combine sliders and spin boxes
    QDoubleSlider* doubleSlider1;
    QDoubleSlider* doubleSlider2;
};


#endif