/*
   SPDX-FileCopyrightText: 2023 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#pragma once

#include <plugins/pluginauthentication.h>

class GitHubPluginAuthentication : public PluginAuthentication
{
    Q_OBJECT
public:
    explicit GitHubPluginAuthentication(QObject *parent = nullptr, const QVariantList & = {});
    ~GitHubPluginAuthentication() override;

    [[nodiscard]] PluginAuthenticationInterface *createInterface(QObject *parent) override;
    [[nodiscard]] AuthenticationManager::AuthMethodType type() const override;
    [[nodiscard]] QString name() const override;
    [[nodiscard]] QString iconName() const override;
};
