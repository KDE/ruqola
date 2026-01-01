/*
   SPDX-FileCopyrightText: 2024-2026 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#pragma once
#include "accountmanager.h"
#include "libruqolacore_export.h"
#include <QWidget>

class LIBRUQOLACORE_EXPORT PluginAuthenticationConfigureWidget : public QWidget
{
    Q_OBJECT
public:
    enum class AuthenticationLoginType : uint8_t {
        Unknown = 0,
        Create,
        Modify,
        Login,
    };
    explicit PluginAuthenticationConfigureWidget(QWidget *parent = nullptr);
    ~PluginAuthenticationConfigureWidget() override;

    [[nodiscard]] virtual AccountManager::AccountManagerInfo accountInfo() = 0;
    virtual void setAccountInfo(const AccountManager::AccountManagerInfo &info) = 0;
    virtual void changeAuthenticationWidgetStatus(bool enabled) = 0;

    void setExistingAccountNames(const QStringList &list);

    virtual void setAuthenticationLoginType(AuthenticationLoginType type);

Q_SIGNALS:
    void settingsIsValid(bool enabled);
    void tryLogin();

protected:
    QStringList mExistingAccountNames;
};
