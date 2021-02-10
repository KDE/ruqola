/*
   Copyright (c) 2021 Laurent Montel <montel@kde.org>

   This library is free software; you can redistribute it and/or modify
   it under the terms of the GNU Library General Public License as published
   by the Free Software Foundation; either version 2 of the License or
   ( at your option ) version 3 or, at the discretion of KDE e.V.
   ( which shall act as a proxy as in section 14 of the GPLv3 ), any later version.

   This library is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
   Library General Public License for more details.

   You should have received a copy of the GNU Library General Public License
   along with this library; see the file COPYING.LIB.  If not, write to
   the Free Software Foundation, Inc., 51 Franklin Street, Fifth Floor,
   Boston, MA 02110-1301, USA.
*/

#include "systemmessagesmodel.h"
#include <KLocalizedString>

SystemMessagesModel::SystemMessagesModel(QObject *parent)
    : QStandardItemModel(parent)
{
    fillModel();
}

SystemMessagesModel::~SystemMessagesModel()
{
}

void SystemMessagesModel::createItem(const QString &displayStr, const QString &systemStr)
{
    auto item = new QStandardItem(displayStr);
    item->setFlags(Qt::ItemIsUserCheckable | Qt::ItemIsEnabled);
    item->setData(systemStr, SystemMessages);
    item->setToolTip(displayStr);
    appendRow(item);
}

void SystemMessagesModel::fillModel()
{
    auto item = new QStandardItem(i18n("System Messages"));
    item->setSelectable(false);
    appendRow(item);
    createItem(i18n("Hide \"User Added\" messages"), QStringLiteral("au"));
    createItem(i18n("Hide \"Welcome\" messages"), QStringLiteral("wm"));
    createItem(i18n("Hide \"User Joined Conversation\" messages"), QStringLiteral("ut"));
    createItem(i18n("Hide \"User Leave\" messages"), QStringLiteral("ul"));
    createItem(i18n("Hide \"User Join\" messages"), QStringLiteral("uj"));
    createItem(i18n("Hide \"Role No Longer Defined\" messages"), QStringLiteral("role_removed"));
    createItem(i18n("Hide \"Was Set Role\" messages"), QStringLiteral("role_added"));
    createItem(i18n("Hide \"User Removed\" messages"), QStringLiteral("ru"));
    createItem(i18n("Hide \"Room Unarchived\" messages"), QStringLiteral("room_unarchived"));
    createItem(i18n("Hide \"Room encryption disabled\" messages"), QStringLiteral("room_disabled_encryption"));
    createItem(i18n("Hide \"Room encryption enabled\" messages"), QStringLiteral("enabled_encryption"));
    createItem(i18n("Hide \"Room type changed\" messages"), QStringLiteral("room_changed_privacy"));
    createItem(i18n("Hide \"Room avatar changed\" messages"), QStringLiteral("room_changed_avatar"));
    createItem(i18n("Hide \"User Muted / Unmuted\" messages"), QStringLiteral("mute_unmute"));
    createItem(i18n("Hide \"Room Name Changed\" messages"), QStringLiteral("r"));
    createItem(i18n("Hide \"Message Removed\" messages"), QStringLiteral("rm"));
    createItem(i18n("Hide \"Room Archived\" messages"), QStringLiteral("room_archived"));
}

void SystemMessagesModel::setMessagesSystem(const QStringList &lst)
{
    // item->setData(Qt::Checked, Qt::CheckStateRole);
    // TODO
}

QStringList SystemMessagesModel::systemMessagesSelected() const
{
    QStringList lst;
    const int rowCountNb = rowCount();
    // First one is not a message type
    for (int i = 1; i < rowCountNb; i++) {
        QStandardItem *itemModel = item(i);
        if (itemModel) {
            if (itemModel->isCheckable() && itemModel->checkState() == Qt::Checked) {
                lst.append(itemModel->data(SystemMessages).toString());
            }
        }
    }
    return lst;
}
