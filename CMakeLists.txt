cmake_minimum_required( VERSION 2.6 )

project( QScientific )


#######################################
# Include Qt
#######################################

find_package( Qt4 REQUIRED )

include( ${QT_USE_FILE} )

include_directories( ${QT_INCLUDE_DIR}
                     ${CMAKE_CURRENT_BINARY_DIR}
                     ${CMAKE_CURRENT_SOURCE_DIR} )

# Set up variables for moc
set( QT_HEADER QDoubleSlider.h QScientificSpinBox.h QDualValue.h QExploratorySlider.h QPowerSlider.h QNonlinearSlider.h )
set( QT_SRC QDoubleSlider.cpp QScientificSpinBox.cpp QDualValue.cpp QExploratorySlider.cpp QPowerSlider.cpp QNonlinearSlider.cpp )

# Do moc stuff
qt4_wrap_cpp( QT_MOC_SRC ${QT_HEADER} )
add_definitions( -DQT_GUI_LIBS -DQT_CORE_LIB -DQT3_SUPPORT )


#######################################
# Include QScientific code
#######################################

set( SRC ${QT_HEADER} ${QT_SRC} ${QT_MOC_SRC} )
add_library( QScientific ${SRC} )


#######################################
# CMake project stuff
#######################################

export( PACKAGE QScientific )

configure_file( UseQScientific.cmake.in "${PROJECT_BINARY_DIR}/UseQScientific.cmake" COPYONLY IMMEDIATE )
configure_file( QScientificConfig.cmake.in "${PROJECT_BINARY_DIR}/QScientificConfig.cmake" @ONLY )


#######################################
# Include application directory
#######################################

add_subdirectory( App )