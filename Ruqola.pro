TEMPLATE = app

QT += widgets gui core qml quick websockets

HEADERS += messagemodel.h roommodel.h ddpclient.h userdata.h rocketchatbackend.h
SOURCES += main.cpp messagemodel.cpp roommodel.cpp ddpclient.cpp userdata.cpp rocketchatbackend.cpp

RESOURCES += qml.qrc

# Additional import path used to resolve QML modules in Qt Creator's code model
# QML_IMPORT_PATH = 

# Default rules for deployment.
include(deployment.pri)

DISTFILES +=
