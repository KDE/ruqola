TEMPLATE = app

QT += widgets gui core qml quick websockets

HEADERS += src/messagemodel.h src/roommodel.h src/ddpclient.h src/userdata.h src/rocketchatbackend.h
SOURCES += main.cpp src/messagemodel.cpp src/roommodel.cpp src/ddpclient.cpp src/userdata.cpp src/rocketchatbackend.cpp

RESOURCES += qml.qrc

CONFIG += c++11

# Additional import path used to resolve QML modules in Qt Creator's code model
# QML_IMPORT_PATH = 

# Default rules for deployment.
include(deployment.pri)

DISTFILES +=
