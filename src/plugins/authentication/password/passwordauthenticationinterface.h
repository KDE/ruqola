/*
   SPDX-FileCopyrightText: 2018-2021 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: GPL-2.0-or-later
*/

#pragma once

#include "plugins/pluginauthenticationinterface.h"

class PasswordAuthenticationInterface : public PluginAuthenticationInterface
{
    Q_OBJECT
public:
    explicit PasswordAuthenticationInterface(QObject *parent = nullptr);
    ~PasswordAuthenticationInterface() override;

    void login() override;
};

