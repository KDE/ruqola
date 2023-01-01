/*
   SPDX-FileCopyrightText: 2019-2023 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#pragma once

#include "libruqola_private_export.h"
#include "users/userbasejob.h"
#include "utils.h"
#include <QObject>
class QTimer;
class RocketChatAccount;
class LIBRUQOLACORE_TESTS_EXPORT AvatarManager : public QObject
{
    Q_OBJECT
public:
    explicit AvatarManager(RocketChatAccount *account, QObject *parent = nullptr);
    ~AvatarManager() override;

    void insertInDownloadQueue(const Utils::AvatarInfo &info);

    RocketChatAccount *account() const;

Q_SIGNALS:
    void insertAvatarUrl(const QString &userId, const QUrl &url);

private:
    void slotInsertAvatarUrl(const Utils::AvatarInfo &info, const QUrl &url);
    void slotLoadNextAvatar();
    void slotRescheduleDownload();
    QVector<Utils::AvatarInfo> mAvatarDownloadIdentifer;
    RocketChatAccount *const mAccount;
    QTimer *const mTimer;
};
