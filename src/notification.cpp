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
#include "ruqola.h"

#include <QAction>
#include <QMenu>
#include <QMessageBox>

Notification::Notification(QObject *parent)
    : QSystemTrayIcon(parent)
    , m_quitAction(nullptr)
    , m_trayIconMenu(nullptr)
{
    if (createTrayIcon()) {
        createActions();
    }
}

//create actions in Menu
void Notification::createActions()
{
    m_quitAction = new QAction(tr("&Quit"), this);
    connect(m_quitAction, &QAction::triggered, qApp, &QCoreApplication::quit);
    m_trayIconMenu = new QMenu();
    m_trayIconMenu->addAction(m_quitAction);
    m_trayIconMenu->addSeparator();

    setContextMenu(m_trayIconMenu);
}

//create systrayIcon
bool Notification::createTrayIcon()
{
    if (!QSystemTrayIcon::isSystemTrayAvailable()) {
        QMessageBox::critical(nullptr, QObject::tr("Systray"), QObject::tr("Cannot detect SystemTray on this system."));
        return false;
    }

    setToolTip(QStringLiteral("Ruqola"));
    setIcon(QIcon(QStringLiteral(":/icons/systray.png")));
    setVisible(true);
    return true;
}
