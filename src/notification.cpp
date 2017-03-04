/*
 * <one line to give the program's name and a brief idea of what it does.>
 * Copyright 2016  Riccardo Iaconelli <riccardo@kde.org>
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License as
 * published by the Free Software Foundation; either version 2 of
 * the License or (at your option) version 3 or any later version
 * accepted by the membership of KDE e.V. (or its successor approved
 * by the membership of KDE e.V.), which shall act as a proxy
 * defined in Section 14 of version 3 of the license.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 *
 */

#include "userdata.h"
#include "ddpclient.h"
#include "notification.h"

#include <QAction>
#include <QMenu>
#include <QMessageBox>
#include <QWindow>

bool Notification::IswindowClosed() const {
    return m_windowClosed;
}

void Notification::setWindowClosed(bool val){
    m_windowClosed = val;
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
   m_quitAction = new QAction(tr("&Quit"), this);
    connect(m_quitAction, &QAction::triggered, qApp, &QCoreApplication::quit);
}

void Notification::createTrayIcon(){
    qDebug() << "Creating system tray";
    if (!QSystemTrayIcon::isSystemTrayAvailable()) {
        QMessageBox::critical(0, QObject::tr("Systray"), QObject::tr("Cannot detect SystemTray on this system."));
        return;
    }
    m_trayIconMenu = new QMenu();
    m_trayIconMenu->addAction(m_quitAction);
    m_trayIconMenu->addSeparator();

    m_systrayIcon->setContextMenu(m_trayIconMenu);
    m_systrayIcon->setToolTip("Ruqola");
    m_systrayIcon->setIcon(QIcon(":/systray.png"));
    m_systrayIcon->setVisible(true);
}

void Notification::showNotification(const QString userName, QString message )
{
    if ( Notification::IswindowClosed() && ( UserData::self()->loginStatus() == DDPClient::LoggedIn) ){
    QString title("New Message"); //This can be enhanced later
    QString msg = QString("%1 \n %2").arg(userName).arg(message);
    m_systrayIcon->showMessage(title, msg, QSystemTrayIcon::Information, 5000 );
    }
}

Notification::Notification(): m_windowClosed(false)
{
    qDebug() << "Called notification constructor";
}

void Notification::iconActivated(QSystemTrayIcon::ActivationReason reason){
    Q_UNUSED(reason);
    qDebug() << "Icon activated";
}

/*

Notification *Notification::n_self = 0;

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
*/
