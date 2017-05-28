#include <QApplication>
#include <QtQml>

// only if desktop
#include "src/roommodel.h"
#include "src/rocketchatbackend.h"
#include "src/ruqola.h"
#include "src/notification.h"

#include <QDebug>
#include <QtCore/QJsonDocument>
#include <QtCore>

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);
    app.setWindowIcon(QIcon(":/systray.png"));
    
    QCoreApplication::setOrganizationName("KDE");
    QCoreApplication::setOrganizationDomain("kde.org");
    QCoreApplication::setApplicationName("Ruqola");
    
    qmlRegisterSingletonType<Ruqola>("KDE.Ruqola.Ruqola", 1, 0, "Ruqola", ruqola_singletontype_provider);
    qmlRegisterType<MessageModel>("KDE.Ruqola.MessageModel", 1, 0, "MessageModel");
    qmlRegisterType<DDPClient>("KDE.Ruqola.DDPClient", 1, 0, "DDPClient");
    qmlRegisterType<RoomModel>("KDE.Ruqola.RoomModel", 1, 0, "RoomModel");
    qmlRegisterType<RoomWrapper>("KDE.Ruqola.RoomWrapper", 1, 0, "RoomWrapper");
    qmlRegisterType<Notification>("KDE.Ruqola.Notification", 1, 0, "Notification");

    RocketChatBackend c;
    QQmlApplicationEngine engine;

    QQmlContext *ctxt = engine.rootContext();
    ctxt->setContextProperty("systrayIcon", Ruqola::self()->notification());
//    ctxt->setContextProperty("attachmentButton", Ruqola::self()->attachmentButtonClicked());

    engine.load(QUrl(QStringLiteral("qrc:/main.qml")));

    if (engine.rootObjects().isEmpty())
        return -1;

    return app.exec();
}
