/*
   SPDX-FileCopyrightText: 2024 Laurent Montel <montel.org>

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

    void login() override;
    [[nodiscard]] bool showConfigureDialog(QWidget *parent) override;
    [[nodiscard]] virtual PluginAuthenticationConfigureWidget *configureWidget(QWidget *parent) override;
};
