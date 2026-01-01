/*
   SPDX-FileCopyrightText: 2024-2026 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#pragma once

#include "plugins/pluginauthenticationinterface.h"

class PersonalAccessTokenAuthenticationInterface : public PluginAuthenticationInterface
{
    Q_OBJECT
public:
    explicit PersonalAccessTokenAuthenticationInterface(QObject *parent = nullptr);
    ~PersonalAccessTokenAuthenticationInterface() override;

    [[nodiscard]] bool login() override;
    [[nodiscard]] virtual PluginAuthenticationConfigureWidget *configureWidget(QWidget *parent) override;
};
