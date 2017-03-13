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

bool Notification::windowClosed() const {
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

//   QWindow window;
//   m_restore = new QAction(tr("&Restore"),this);
//   connect(m_restore, &QAction::triggered, window, &QWindow::showMaximized );
}

void Notification::createTrayIcon(){
    if (!QSystemTrayIcon::isSystemTrayAvailable()) {
        QMessageBox::critical(0, QObject::tr("Systray"), QObject::tr("Cannot detect SystemTray on this system."));
        return;
    }
    m_trayIconMenu = new QMenu();
    m_trayIconMenu->addAction(m_quitAction);
    m_trayIconMenu->addSeparator();
//    m_trayIconMenu->addAction(m_restore);

    m_systrayIcon->setContextMenu(m_trayIconMenu);
    m_systrayIcon->setToolTip("Ruqola");
    m_systrayIcon->setIcon(QIcon(":/systray.png"));
    m_systrayIcon->setVisible(true);
}

void Notification::toggle(){
    m_restore->setEnabled(windowClosed());
    m_systrayIcon->setVisible(windowClosed());
}


Notification::Notification(): m_windowClosed(false)
{
    m_systrayIcon = new QSystemTrayIcon();
    createActions();
    createTrayIcon();
    connect(m_systrayIcon, SIGNAL(activated(QSystemTrayIcon::ActivationReason)),
                this, SLOT(iconActivated(QSystemTrayIcon::ActivationReason)));
}

void Notification::iconActivated(QSystemTrayIcon::ActivationReason reason){
    qDebug() << "Icon activated";
    if (reason == QSystemTrayIcon::Trigger) {
        this->show(); //isn't working
        /*
         * this->raise();
           this->activateWindow();
           this->showNormal();
        */
        m_windowClosed = false;
        toggle();
    }
}
