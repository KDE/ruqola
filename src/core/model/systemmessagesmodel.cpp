/*
   SPDX-FileCopyrightText: 2021-2022 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "systemmessagesmodel.h"
#include "ruqola_debug.h"
#include <KLocalizedString>

SystemMessagesModel::SystemMessagesModel(QObject *parent)
    : QStandardItemModel(parent)
{
    fillModel();
}

SystemMessagesModel::~SystemMessagesModel() = default;

void SystemMessagesModel::createItem(const QString &displayStr, const QString &systemStr)
{
    auto item = new QStandardItem(displayStr);
    item->setFlags(Qt::ItemIsUserCheckable | Qt::ItemIsEnabled);
    item->setData(systemStr, SystemMessages);
    item->setData(Qt::Unchecked, Qt::CheckStateRole);
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
    createItem(i18n("Hide \"User Left Team\" messages"), QStringLiteral("ult"));
    createItem(i18n("Hide \"User Join\" messages"), QStringLiteral("uj"));
    createItem(i18n("Hide \"Role No Longer Defined\" messages"), QStringLiteral("subscription-role-removed"));
    createItem(i18n("Hide \"Was Set Role\" messages"), QStringLiteral("subscription-role-added"));
    createItem(i18n("Hide \"User Removed\" messages"), QStringLiteral("ru"));
    createItem(i18n("Hide \"Room Unarchived\" messages"), QStringLiteral("room-unarchived"));
    createItem(i18n("Hide \"Room encryption disabled\" messages"), QStringLiteral("room_disabled_encryption"));
    createItem(i18n("Hide \"Room encryption enabled\" messages"), QStringLiteral("enabled_encryption"));
    createItem(i18n("Hide \"Room type changed\" messages"), QStringLiteral("room_changed_privacy"));
    createItem(i18n("Hide \"Room avatar changed\" messages"), QStringLiteral("room_changed_avatar"));
    createItem(i18n("Hide \"User Muted / Unmuted\" messages"), QStringLiteral("mute_unmute"));
    createItem(i18n("Hide \"Room Name Changed\" messages"), QStringLiteral("r"));
    createItem(i18n("Hide \"Message Removed\" messages"), QStringLiteral("rm"));
    createItem(i18n("Hide \"Room Archived\" messages"), QStringLiteral("room-archived"));
    createItem(i18n("Hide \"Encryption Enabled\" messages"), QStringLiteral("room_e2e_enabled"));
    createItem(i18n("Hide \"Encryption Disabled\" messages"), QStringLiteral("room_e2e_disabled"));
    createItem(i18n("Hide \"Room added writing permission\" messages"), QStringLiteral("room-removed-read-only"));
    createItem(i18n("Hide \"Room set read-only\" messages"), QStringLiteral("room-set-read-only"));
    createItem(i18n("Hide \"Room disallowed reacting\" messages"), QStringLiteral("room-disallowed-reacting"));
    createItem(i18n("Hide \"Room allowed reacting\" messages"), QStringLiteral("room-allowed-reacting"));
    createItem(i18n("Hide \"User Added Room to Team\" messages"), QStringLiteral("user-added-room-to-team"));
    createItem(i18n("Hide \"User converted team to channel\" messages"), QStringLiteral("user-converted-to-channel"));
    createItem(i18n("Hide \"User converted channel to a Team\" messages"), QStringLiteral("user-converted-to-team"));
    createItem(i18n("Hide \"User deleted room from Team\" messages"), QStringLiteral("user-deleted-room-from-team"));
    createItem(i18n("Hide \"User removed room from Team\" messages"), QStringLiteral("user-removed-room-from-team"));

    createItem(i18n("Hide \"Removed user from Team\" messages"), QStringLiteral("removed-user-from-team"));
    createItem(i18n("Hide \"Added user to Team\" messages"), QStringLiteral("added-user-to-team"));
    createItem(i18n("Hide \"Room topic changed\" messages"), QStringLiteral("room_changed_topic"));

    createItem(i18n("Joined this Team"), QStringLiteral("ujt"));
}

void SystemMessagesModel::setMessagesSystem(const QStringList &lst)
{
    const int rowCountNb = rowCount();
    // First one is not a message type
    QStringList copyList = lst;
    for (int i = 1; i < rowCountNb; i++) {
        QStandardItem *itemModel = item(i);
        if (itemModel) {
            for (const QString &s : lst) {
                if (itemModel->data(SystemMessages).toString() == s) {
                    itemModel->setCheckState(Qt::Checked);
                    copyList.removeAll(s);
                    break;
                }
            }
        }
    }
    if (!copyList.isEmpty()) {
        qCWarning(RUQOLA_LOG) << "System message is not implemented here " << copyList;
    }
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
