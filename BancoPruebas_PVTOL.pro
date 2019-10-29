#-------------------------------------------------------------------------------
# Make options
#-------------------------------------------------------------------------------

UI_DIR = uic
MOC_DIR = moc
RCC_DIR = qrc
OBJECTS_DIR = obj

CONFIG += c++11

#-------------------------------------------------------------------------------
# Qt configuration
#-------------------------------------------------------------------------------

TEMPLATE = app
TARGET = pvtol

CONFIG += qtc_runnable
CONFIG += resources_big
CONFIG += qtquickcompiler

QT += xml
QT += svg
QT += sql
QT += core
QT += quick
QT += location
QT += concurrent
QT += serialport
QT += positioning
QT += quickcontrols2

QTPLUGIN += qsvg

#-------------------------------------------------------------------------------
# Import source code
#-------------------------------------------------------------------------------

HEADERS += \
    src/Serial.h

SOURCES += \
    src/Serial.cpp \
    src/main.cpp

RESOURCES += \
    images/images.qrc \
    qml/qml.qrc
