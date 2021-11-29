/*
   SPDX-FileCopyrightText: 2018-2021 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: GPL-2.0-or-later
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
