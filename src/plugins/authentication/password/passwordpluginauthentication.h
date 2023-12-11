/*
   SPDX-FileCopyrightText: 2018-2023 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#pragma once

#include <plugins/pluginauthentication.h>

class PasswordPluginAuthentication : public PluginAuthentication
{
    Q_OBJECT
public:
    explicit PasswordPluginAuthentication(QObject *parent = nullptr, const QVariantList & = {});
    ~PasswordPluginAuthentication() override;

    [[nodiscard]] PluginAuthenticationInterface *createInterface(QObject *parent) override;
    [[nodiscard]] AuthenticationManager::OauthType type() const override;
    [[nodiscard]] QString name() const override;
    [[nodiscard]] QString iconName() const override;
};
