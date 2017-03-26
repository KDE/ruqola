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

#include "notification.h"
#include "userdata.h"

#include <QAction>
#include <QMenu>
#include <QMessageBox>

bool Notification::windowVisible() const {
    return m_windowVisible;
}

void Notification::setWindowVisible(bool value){
    if (m_windowVisible != value){
        m_windowVisible = value;
        emit windowVisibleChanged();
    }
}

QString Notification::message() const
{
    return m_message;
}

void Notification::setMessage(const QString &message)
{
    if (m_message != message){
        m_message = message;
        emit messageChanged();
    }
}


//create actions in Menu
void Notification::createActions(){
    m_quitAction = new QAction(tr("&Quit"), this);
    connect(m_quitAction, &QAction::triggered, qApp, &QCoreApplication::quit);
}

//create systrayIcon
void Notification::createTrayIcon(){

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

void Notification::updateDesktopNotification() {

    if (!windowVisible()){
    QString title("New Ruqola Message!"); //This can be enhanced later
    this->showMessage(title, m_message, QSystemTrayIcon::Information, 5000 );
    }
}


Notification::Notification(): m_windowVisible(true){
    createActions();
    createTrayIcon();

    //connect messageChanged signal to updateDesktopNotification Slot
    connect(this, SIGNAL(messageChanged()), this, SLOT(updateDesktopNotification()));

}
