/*
   SPDX-FileCopyrightText: 2020-2024 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#pragma once

#include "libruqolawidgets_export.h"
#include <QWidget>
class QLabel;
class RocketChatAccount;
class ServerConfigInfo;
class LIBRUQOLAWIDGETS_EXPORT ServerInfoWidget : public QWidget
{
    Q_OBJECT
public:
    explicit ServerInfoWidget(RocketChatAccount *account, QWidget *parent = nullptr);
    ~ServerInfoWidget() override;

private:
    LIBRUQOLAWIDGETS_NO_EXPORT void setServerConfigInfo(ServerConfigInfo *info);
    QLabel *const mAccountName;
    QLabel *const mUserName;
    QLabel *const mServerVersion;
    QLabel *const mServerUrl;
    QLabel *mEnterpriseLicense = nullptr;
    QLabel *const mLogo;
    RocketChatAccount *const mRocketChatAccount;
};
