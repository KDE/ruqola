/*
   SPDX-FileCopyrightText: 2021-2025 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "systemmessagesmodel.h"
using namespace Qt::Literals::StringLiterals;

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
    auto item = new QStandardItem(i18n("Select Messages to hide"));
    item->setSelectable(false);
    appendRow(item);
    createItem(i18n("Hide \"User Added\" messages"), u"au"_s);
    createItem(i18n("Hide \"Welcome\" messages"), u"wm"_s);
    createItem(i18n("Hide \"User Joined Conversation\" messages"), u"ut"_s);
    createItem(i18n("Hide \"User Leave\" messages"), u"ul"_s);
    createItem(i18n("Hide \"User Left Team\" messages"), u"ult"_s);
    createItem(i18n("Hide \"User Join\" messages"), u"uj"_s);
    createItem(i18n("Hide \"Role No Longer Defined\" messages"), u"subscription-role-removed"_s);
    createItem(i18n("Hide \"Was Set Role\" messages"), u"subscription-role-added"_s);
    createItem(i18n("Hide \"User Removed\" messages"), u"ru"_s);
    createItem(i18n("Hide \"Room Unarchived\" messages"), u"room-unarchived"_s);
    createItem(i18n("Hide \"Room encryption disabled\" messages"), u"room_disabled_encryption"_s);
    createItem(i18n("Hide \"Room encryption enabled\" messages"), u"enabled_encryption"_s);
    createItem(i18n("Hide \"Room type changed\" messages"), u"room_changed_privacy"_s);
    createItem(i18n("Hide \"Room avatar changed\" messages"), u"room_changed_avatar"_s);
    createItem(i18n("Hide \"User Muted / Unmuted\" messages"), u"mute_unmute"_s);
    createItem(i18n("Hide \"Room Name Changed\" messages"), u"r"_s);
    createItem(i18n("Hide \"Message Removed\" messages"), u"rm"_s);
    createItem(i18n("Hide \"Room Archived\" messages"), u"room-archived"_s);
    createItem(i18n("Hide \"Encryption Enabled\" messages"), u"room_e2e_enabled"_s);
    createItem(i18n("Hide \"Encryption Disabled\" messages"), u"room_e2e_disabled"_s);
    createItem(i18n("Hide \"Room added writing permission\" messages"), u"room-removed-read-only"_s);
    createItem(i18n("Hide \"Room set read-only\" messages"), u"room-set-read-only"_s);
    createItem(i18n("Hide \"Room disallowed reacting\" messages"), u"room-disallowed-reacting"_s);
    createItem(i18n("Hide \"Room allowed reacting\" messages"), u"room-allowed-reacting"_s);
    createItem(i18n("Hide \"User Added Room to Team\" messages"), u"user-added-room-to-team"_s);
    createItem(i18n("Hide \"User converted team to channel\" messages"), u"user-converted-to-channel"_s);
    createItem(i18n("Hide \"User converted channel to a Team\" messages"), u"user-converted-to-team"_s);
    createItem(i18n("Hide \"User deleted room from Team\" messages"), u"user-deleted-room-from-team"_s);
    createItem(i18n("Hide \"User removed room from Team\" messages"), u"user-removed-room-from-team"_s);

    createItem(i18n("Hide \"Removed user from Team\" messages"), u"removed-user-from-team"_s);
    createItem(i18n("Hide \"Added user to Team\" messages"), u"added-user-to-team"_s);
    createItem(i18n("Hide \"Room topic changed\" messages"), u"room_changed_topic"_s);
    createItem(i18n("Hide \"Room announcement changed\" messages"), u"room_changed_announcement"_s);
    createItem(i18n("Hide \"Room description changed\" messages"), u"room_changed_description"_s);

    createItem(i18n("Joined this Team"), u"ujt"_s);
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

#include "moc_systemmessagesmodel.cpp"
