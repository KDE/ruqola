/*
   SPDX-FileCopyrightText: 2023 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#pragma once

#include "authenticationinfo.h"
#include "authenticationmanager.h"
#include "libruqolawidgets_private_export.h"
#include <QWidget>

class RocketChatAccount;
class QVBoxLayout;
class LIBRUQOLAWIDGETS_TESTS_EXPORT AuthenticationOauthWidget : public QWidget
{
    Q_OBJECT
public:
    explicit AuthenticationOauthWidget(QWidget *parent = nullptr);
    ~AuthenticationOauthWidget() override;

    void setAuthenticationInfos(const QVector<AuthenticationInfo> &authenticationInfos);

Q_SIGNALS:
    void authentication(AuthenticationManager::AuthMethodType type);

private:
    QVBoxLayout *const mMainLayout;
};
