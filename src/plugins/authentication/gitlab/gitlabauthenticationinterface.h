/*
   SPDX-FileCopyrightText: 2023-2024 Laurent Montel <montel.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#pragma once

#include "plugins/pluginauthenticationinterface.h"

class GitLabAuthenticationInterface : public PluginAuthenticationInterface
{
    Q_OBJECT
public:
    explicit GitLabAuthenticationInterface(QObject *parent = nullptr);
    ~GitLabAuthenticationInterface() override;

    void login() override;
};
