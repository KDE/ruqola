/*
   SPDX-FileCopyrightText: 2023 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "serverscombobox.h"
#include <KLocalizedString>

ServersComboBox::ServersComboBox(QWidget *parent)
    : QComboBox(parent)
{
    connect(this, &QComboBox::activated, this, &ServersComboBox::slotSelectAccount);
}

ServersComboBox::~ServersComboBox() = default;

void ServersComboBox::addServerList(const QStringList &serverNames)
{
    addItem(i18n("Filter Account..."), QString());
    for (const auto &account : serverNames) {
        addItem(account, account);
    }
    setSizeAdjustPolicy(QComboBox::AdjustToContents);
}

void ServersComboBox::slotSelectAccount(int index)
{
    if (index != -1) {
        const QString accountName = itemData(index).toString();
        Q_EMIT accountSelected(accountName);
    }
}
