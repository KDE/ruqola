/*
   SPDX-FileCopyrightText: 2023-2026 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#pragma once

#include "plugins/pluginauthentication.h"

class PersonalAccessTokenPluginAuthentication : public PluginAuthentication
{
    Q_OBJECT
public:
    explicit PersonalAccessTokenPluginAuthentication(QObject *parent = nullptr, const QVariantList & = {});
    ~PersonalAccessTokenPluginAuthentication() override;

    [[nodiscard]] PluginAuthenticationInterface *createInterface(QObject *parent) override;
    [[nodiscard]] AuthenticationManager::AuthMethodType authenticationType() const override;
    [[nodiscard]] QString name() const override;
    [[nodiscard]] QString iconName() const override;
};
