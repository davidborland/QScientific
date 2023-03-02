/*=========================================================================

  Name:        QScientificTest.cpp

  Author:      David Borland

  Copyright:   The Renaissance Computing Institute (RENCI)

  Description: Test application for QScientific widgets.

=========================================================================*/


#include "MainWindow.h"

#include <QApplication>


int main(int argc, char** argv) {
    // Initialize Qt
    QApplication app(argc, argv);

    MainWindow mainWindow;
    mainWindow.show();

    return app.exec();
}