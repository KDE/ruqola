/*
   SPDX-FileCopyrightText: 2023 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "serverscombobox.h"
#include "accountmanager.h"
#include "ruqola.h"
ServersComboBox::ServersComboBox(QWidget *parent)
    : QComboBox(parent)
{
    generateServerList();
}

ServersComboBox::~ServersComboBox() = default;

void ServersComboBox::addServerList(const QStringList &serverNames)
{
    addItems(QStringList() << QString() << serverNames);
    setSizeAdjustPolicy(QComboBox::AdjustToContents);
}

void ServersComboBox::generateServerList()
{
    QStringList lst;
    auto model = Ruqola::self()->accountManager()->rocketChatAccountProxyModel();
    for (int i = 0; i < model->rowCount(); ++i) {
        const auto index = model->index(i, 0);
        auto account = index.data(RocketChatAccountModel::Account).value<RocketChatAccount *>();
        lst << account->displayName();
    }
    addServerList(lst);
}
