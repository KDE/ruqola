/*
   SPDX-FileCopyrightText: 2018-2023 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "pluginauthenticationinterface.h"

#include "rocketchataccount.h"

PluginAuthenticationInterface::PluginAuthenticationInterface(QObject *parent)
    : QObject(parent)
{
}

PluginAuthenticationInterface::~PluginAuthenticationInterface() = default;

RocketChatAccount *PluginAuthenticationInterface::account() const
{
    return mAccount;
}

void PluginAuthenticationInterface::setAccount(RocketChatAccount *account)
{
    mAccount = account;
}
