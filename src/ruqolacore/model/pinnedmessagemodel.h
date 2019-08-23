/*
   Copyright (c) 2019 Montel Laurent <montel@kde.org>

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

#ifndef PINNEDMESSAGEMODEL_H
#define PINNEDMESSAGEMODEL_H

#include "libruqola_private_export.h"
#include "messagemodel.h"

class LIBRUQOLACORE_TESTS_EXPORT PinnedMessageModel : public MessageModel
{
    Q_OBJECT
public:
    explicit PinnedMessageModel(const QString &roomID = QStringLiteral("no_room"), RocketChatAccount *account = nullptr, Room *room = nullptr, QObject *parent = nullptr);
    ~PinnedMessageModel();

    Q_REQUIRED_RESULT QString roomId() const;
    void setRoomId(const QString &roomId);

    void parsePinnedMessages(const QJsonObject &obj);
    void loadMorePinnedMessages(const QJsonObject &obj);

    Q_INVOKABLE Q_REQUIRED_RESULT int total() const;
    void setTotal(int total);

    Q_REQUIRED_RESULT bool loadMorePinnedMessageInProgress() const;
    void setLoadMorePinnedMessageInProgress(bool inProgress);

private:
    void parse(const QJsonObject &obj);
    QString mRoomId;
    int mTotal = 0;
    bool mLoadingInProgress = false;
};

#endif // PINNEDMESSAGEMODEL_H
