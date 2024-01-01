/*
   SPDX-FileCopyrightText: 2023-2024 Laurent Montel <montel.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#pragma once

#include "plugins/pluginauthenticationinterface.h"

class GitHubAuthenticationInterface : public PluginAuthenticationInterface
{
    Q_OBJECT
public:
    explicit GitHubAuthenticationInterface(QObject *parent = nullptr);
    ~GitHubAuthenticationInterface() override;

    void login() override;
};
