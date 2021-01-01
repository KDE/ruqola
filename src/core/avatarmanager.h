/*
   Copyright (c) 2019-2021 Laurent Montel <montel@kde.org>

   This library is free software; you can redistribute it and/or modify
   it under the terms of the GNU Library General Public License as published
   by the Free Software Foundation; either version 2 of the License or
   ( at your option ) version 3 or, at the discretion of KDE e.V.
   ( which shall act as a proxy as in section 14 of the GPLv3 ), any later version.

   This library is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
   Library General Public License for more details.

   You should have received a copy of the GNU Library General Public License
   along with this library; see the file COPYING.LIB.  If not, write to
   the Free Software Foundation, Inc., 51 Franklin Street, Fifth Floor,
   Boston, MA 02110-1301, USA.
*/

#ifndef AVATARMANAGER_H
#define AVATARMANAGER_H

#include <QObject>
#include "users/userbasejob.h"
#include "libruqola_private_export.h"
#include "utils.h"
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
    QTimer *mTimer = nullptr;
};

#endif // AVATARMANAGER_H
