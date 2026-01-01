/*
   SPDX-FileCopyrightText: 2023-2026 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#pragma once

#include "authenticationmanager.h"
#include "libruqolawidgets_private_export.h"
#include <QWidget>

class AuthenticationInfo;
class QVBoxLayout;
class AuthenticationButton;
class LIBRUQOLAWIDGETS_TESTS_EXPORT AuthenticationOauthWidget : public QWidget
{
    Q_OBJECT
public:
    explicit AuthenticationOauthWidget(QWidget *parent = nullptr);
    ~AuthenticationOauthWidget() override;

    void addAuthenticationMethod(const AuthenticationInfo &info);
    void clear();

Q_SIGNALS:
    void authentication(AuthenticationManager::AuthMethodType type);

private:
    QVBoxLayout *const mMainLayout;
    QList<AuthenticationButton *> mAuthenticationButtonList;
};
