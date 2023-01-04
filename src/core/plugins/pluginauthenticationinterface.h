/*
   SPDX-FileCopyrightText: 2018-2023 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#pragma once

#include "libruqolacore_export.h"
#include <QObject>
class RocketChatAccount;
class LIBRUQOLACORE_EXPORT PluginAuthenticationInterface : public QObject
{
    Q_OBJECT
public:
    explicit PluginAuthenticationInterface(QObject *parent = nullptr);
    ~PluginAuthenticationInterface() override;

    virtual void login() = 0;

    Q_REQUIRED_RESULT RocketChatAccount *account() const;
    void setAccount(RocketChatAccount *account);

Q_SIGNALS:
    void loginFailed(const QString &message);

protected:
    RocketChatAccount *mAccount = nullptr;
};
