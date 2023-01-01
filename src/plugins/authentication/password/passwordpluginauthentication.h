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

    PluginAuthenticationInterface *createInterface(QObject *parent) override;
    Q_REQUIRED_RESULT AuthenticationManager::OauthType type() const override;
    Q_REQUIRED_RESULT QString name() const override;
    Q_REQUIRED_RESULT QString iconName() const override;
};
