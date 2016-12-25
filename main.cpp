#include <QApplication>
#include <QtQml>
// #include <QtQml/QQmlApplicationEngine>
// #include <QtQml/QQmlContext>

#include "src/roommodel.h"
#include "src/rocketchatbackend.h"
#include "src/userdata.h"

#include <QDebug>
#include <QtCore/QJsonDocument>


int main(int argc, char *argv[])
{
    QApplication app(argc, argv);
    QCoreApplication::setOrganizationName("KDE");
    QCoreApplication::setOrganizationDomain("kde.org");
    QCoreApplication::setApplicationName("Ruqola");
//     DDPClient client();
    
    qmlRegisterSingletonType<UserData>("KDE.Ruqola.UserData", 1, 0, "UserData", userdata_singletontype_provider);

    qmlRegisterType<MessageModel>("KDE.Ruqola.Models", 1, 0, "MessageModel");
    qmlRegisterType<DDPClient>("KDE.Ruqola.DDPClient", 1, 0, "DDPClient");
    qmlRegisterType<RoomModel>("KDE.Ruqola.Models", 1, 0, "RoomModel");

    RocketChatBackend c;
    
    QQmlApplicationEngine engine;
//     QQmlContext *ctxt = engine.rootContext();
//     
//     ctxt->setContextProperty("myModel", UserData::instance()->messageList());
//     ctxt->setContextProperty("roomModel", UserData::instance()->roomModel());
    engine.load(QUrl(QStringLiteral("qrc:/main.qml")));

    return app.exec();
}
