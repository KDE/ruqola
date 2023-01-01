/*
   SPDX-FileCopyrightText: 2018-2023 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#pragma once

#include "authenticationmanager.h"
#include "libruqolacore_export.h"
#include <QObject>

class PluginAuthenticationInterface;

class LIBRUQOLACORE_EXPORT PluginAuthentication : public QObject
{
    Q_OBJECT
public:
    explicit PluginAuthentication(QObject *parent = nullptr);
    ~PluginAuthentication() override;

    virtual PluginAuthenticationInterface *createInterface(QObject *parent) = 0;

    Q_REQUIRED_RESULT virtual AuthenticationManager::OauthType type() const = 0;

    Q_REQUIRED_RESULT virtual QString name() const = 0;
    Q_REQUIRED_RESULT virtual QString iconName() const = 0;
};
