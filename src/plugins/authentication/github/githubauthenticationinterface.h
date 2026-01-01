/*
   SPDX-FileCopyrightText: 2023-2026 Laurent Montel <montel@kde.org>

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

    [[nodiscard]] bool login() override;
    [[nodiscard]] virtual PluginAuthenticationConfigureWidget *configureWidget(QWidget *parent) override;
};
