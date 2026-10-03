/*
   SPDX-FileCopyrightText: 2020-2026 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#pragma once

#include "libruqolawidgets_private_export.h"
#include <QWidget>
class QLabel;
class RocketChatAccount;
class ServerConfigInfo;
class LIBRUQOLAWIDGETS_TESTS_EXPORT ServerInfoWidget : public QWidget
{
    Q_OBJECT
public:
    explicit ServerInfoWidget(RocketChatAccount *account, QWidget *parent = nullptr);
    ~ServerInfoWidget() override;

private:
    LIBRUQOLAWIDGETS_NO_EXPORT void setServerConfigInfo(ServerConfigInfo *info);
    LIBRUQOLAWIDGETS_NO_EXPORT void slotFileDownloaded(const QString &filePath, const QUrl &cacheImageUrl);
    LIBRUQOLAWIDGETS_NO_EXPORT void setCurrentIconPath(const QString &path);
    QLabel *const mAccountName;
    QLabel *const mUserName;
    QLabel *const mServerVersion;
    QLabel *const mServerUrl;
    QLabel *mEnterpriseLicense = nullptr;
    QLabel *const mLogo;
    RocketChatAccount *const mRocketChatAccount;
};
