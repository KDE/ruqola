/*
   SPDX-FileCopyrightText: 2018-2025 Laurent Montel <montel@kde.org>

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

    [[nodiscard]] virtual AuthenticationManager::AuthMethodType authenticationType() const = 0;

    [[nodiscard]] virtual QString name() const = 0;
    [[nodiscard]] virtual QString iconName() const = 0;
};
