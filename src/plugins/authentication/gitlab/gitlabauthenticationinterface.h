/*
   SPDX-FileCopyrightText: 2023-2025 Laurent Montel <montel@kde.org>

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

    [[nodiscard]] bool login() override;
    [[nodiscard]] virtual PluginAuthenticationConfigureWidget *configureWidget(QWidget *parent) override;
};
