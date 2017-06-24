TEMPLATE = app

QT += widgets gui core qml quick websockets network networkauth

HEADERS += src/messagemodel.h src/roommodel.h src/ddpclient.h src/ruqola.h src/rocketchatbackend.h \
    src/notification.h \
    src/messagequeue.h \
    src/authentication.h
SOURCES += main.cpp src/messagemodel.cpp src/roommodel.cpp src/ddpclient.cpp src/ruqola.cpp src/rocketchatbackend.cpp \
    src/notification.cpp \
    src/messagequeue.cpp \
    src/authentication.cpp

RESOURCES += qml.qrc

# Additional import path used to resolve QML modules in Qt Creator's code model
# QML_IMPORT_PATH = 

# Default rules for deployment.
include(deployment.pri)

DISTFILES += \
    src/client_secret.json
