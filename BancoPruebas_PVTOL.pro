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
TARGET = PVTOL

CONFIG += qtc_runnable
CONFIG += resources_big
CONFIG += qtquickcompiler

QT += xml
QT += svg
QT += sql
QT += core
QT += quick
QT += charts
QT += serialport
QT += quickcontrols2

QTPLUGIN += qsvg

#-------------------------------------------------------------------------------
# Import source code
#-------------------------------------------------------------------------------

HEADERS += \
    src/DataManager.h \
    src/Serial.h

SOURCES += \
    src/DataManager.cpp \
    src/Serial.cpp \
    src/main.cpp

RESOURCES += \
    images/images.qrc \
    qml/qml.qrc
