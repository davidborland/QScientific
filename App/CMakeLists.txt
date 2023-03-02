project( QScientificTest )

set( EXECUTABLE_OUTPUT_PATH "${QScientific_BINARY_DIR}/bin" )


#######################################
# Include Qt
#######################################

# Set up variables for moc
set( QT_TEST_UI MainWindow.ui )
#set( QT_TEST_QRC QScientificTest.qrc )
set( QT_TEST_HEADER MainWindow.h )
set( QT_TEST_SRC MainWindow.cpp )

# Do moc stuff
qt4_wrap_ui( QT_TEST_UI_HEADER ${QT_TEST_UI} )
#qt4_add_resources( QT_TEST_RCC_SRC ${QT_TEST_QRC} )
qt4_wrap_cpp( QT_TEST_MOC_SRC ${QT_TEST_HEADER} )
add_definitions( -DQT_GUI_LIBS -DQT_CORE_LIB -DQT3_SUPPORT )
set_source_files_properties( ${QT_TEST_SRC} PROPERTIES OBJECT_DEPENDS "${QT_TEST_UI_HEADER}" )


#######################################
# Include QScientificTest code
#######################################

include_directories( ${QScientificTest_BINARY_DIR} )

set( TEST_SRC QScientificTest.cpp ) 

add_executable( QScientificTest ${QT_TEST_HEADER} ${QT_TEST_SRC} ${QT_TEST_MOC_SRC} ${TEST_SRC} )
add_dependencies( QScientificTest QScientific )
target_link_libraries( QScientificTest ${QT_LIBRARIES} QScientific )