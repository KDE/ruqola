/*
   SPDX-FileCopyrightText: 2018-2024 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
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
