/*
   SPDX-FileCopyrightText: 2018-2021 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: SPDX-License-Identifier: GPL-2.0-or-later
*/

#pragma once

#include "plugins/pluginauthenticationinterface.h"

class GoogleAuthenticationInterface : public PluginAuthenticationInterface
{
    Q_OBJECT
public:
    explicit GoogleAuthenticationInterface(QObject *parent = nullptr);
    ~GoogleAuthenticationInterface() override;

    void login() override;

private:
    void slotLoginProvider(const QString &credentialToken, const QString &credentialSecret);
};

