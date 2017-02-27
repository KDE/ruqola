#include "userdata.h"
#include "ddpclient.h"
#include "notification.h"

#include <QAction>
#include <QMenu>
#include <QMessageBox>
#include <QWindow>

bool Notification::windowMinimized() const {
    return n_windowMinimized;
}

void Notification::setWindowMinimized(const bool &val){
    n_windowMinimized = val;
}

//Opens the room having new message
//void Notification::notificationClicked(){
    /*
     *      1. Maximize systray
     *      2. switch to unread room
     *
     */
//}

void Notification::createActions(){
   quitAction = new QAction(tr("&Quit"), this);
    connect(quitAction, &QAction::triggered, qApp, &QCoreApplication::quit);
}

void Notification::createTrayIcon(){
    qDebug() << "Creating system tray";
    if (!QSystemTrayIcon::isSystemTrayAvailable()) {
        QMessageBox::critical(0, QObject::tr("Systray"), QObject::tr("Cannot detect SystemTray on this system."));
        return;
    }
    trayIconMenu = new QMenu();
    trayIconMenu->addAction(quitAction);
    trayIconMenu->addSeparator();

    n_self->setContextMenu(trayIconMenu);
    n_self->setToolTip("Ruqola");
    n_self->setIcon(QIcon(":/systray.png"));
    n_self->setVisible(true);
}

//void notification_callback(QJsonDocument doc)
//{
//}

void Notification::showNotification(const QString userID, const QString userName, QString message )
{
    Q_UNUSED(userID);
//    QString params = QString("[\"%1\"/\"%2\" ]").arg(userID).arg(QString("notification"));
//    UserData::self()->ddp()->subscribe("stream-notify-user", QJsonDocument::fromJson(params.toLatin1()));

    if ( n_windowMinimized && UserData::self()->loginStatus() == DDPClient::LoggedIn ){
    QString title("New Message"); //This can be enhanced later
    QString msg = QString("%1 \n %2").arg(userName).arg(message);
    n_self->showMessage(title, msg, QSystemTrayIcon::Information, 5000 );
    }
}


Notification *Notification::n_self = 0;

Notification::Notification(): n_windowMinimized(false)
{
    qDebug() << "Called notification constructor";
}

void Notification::iconActivated(QSystemTrayIcon::ActivationReason reason){
    Q_UNUSED(reason);
    qDebug() << "Icon activated";
}


Notification * Notification::self()
{
    qDebug() << "Inside Notification::self()";
    if(!n_self){
        n_self = new Notification;
        n_self->createActions();
        n_self->createTrayIcon();
//        connect(systrayIcon , SIGNAL(activated(QSystemTrayIcon::ActivationReason)),
//                this, SLOT(iconActivated(QSystemTrayIcon::ActivationReason)));
        n_self->show();
    }
    return n_self;
}

