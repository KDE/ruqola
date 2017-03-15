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
#include "notification.h"
#include <QQmlComponent>

#include <QAction>
#include <QMenu>
#include <QMessageBox>
#include <QWindow>

bool Notification::windowClosed() const {
    return m_windowClosed;
}

void Notification::setWindowClosed(bool val){
    qDebug() << "set window closed is called";
    m_windowClosed = val;
    emit windowClosedChanged();
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
    qDebug() << "i m in create action";
   m_quitAction = new QAction(tr("&Quit"), this);
   connect(m_quitAction, &QAction::triggered, qApp, &QCoreApplication::quit);

//   QWindow window;
//   m_restore = new QAction(tr("&Restore"),this);
//   connect(m_restore, &QAction::triggered, window, &QWindow::showMaximized );
}

void Notification::createTrayIcon(){

    qDebug() << "i m in create tray";
    if (!QSystemTrayIcon::isSystemTrayAvailable()) {
        QMessageBox::critical(0, QObject::tr("Systray"), QObject::tr("Cannot detect SystemTray on this system."));
        return;
    }
    m_trayIconMenu = new QMenu();
    m_trayIconMenu->addAction(m_quitAction);
    m_trayIconMenu->addSeparator();
    this->setContextMenu(m_trayIconMenu);
    this->setToolTip("Ruqola");
    this->setIcon(QIcon(":/systray.png"));
    this->setVisible(true);

}

Notification::Notification(): m_windowClosed(false){
    qDebug() << "i m in constructor";
//    m_systrayIcon = new QSystemTrayIcon();
    createActions();
    createTrayIcon();
    connect(this, SIGNAL(activated(QSystemTrayIcon::ActivationReason)),
                this, SLOT(iconActivated(QSystemTrayIcon::ActivationReason)));
}

Notification::~Notification(){
    qDebug() << "notification descructor called for " << this;

}


void Notification::iconActivated(QSystemTrayIcon::ActivationReason reason){
    qDebug() << "Icon activated";

    if (reason == QSystemTrayIcon::Trigger) {
        this->show();
/*
        QQmlEngine engine;
        QQmlComponent component(&engine, QUrl::fromLocalFile("Desktop.qml"));
        QObject *object = component.create();
        QMetaObject::invokeMethod(object, "toggleShow");
        delete object;
*/
        qDebug() << "window closed is " << m_windowClosed;
        if (m_windowClosed){
           m_windowClosed = false;
//           raise();
//           activateWindow();
//           showNormal();
        } else {
            m_windowClosed = true;
 //           hide();
        }
    }
}
