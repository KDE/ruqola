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

#ifndef THREADMESSAGEMODEL_H
#define THREADMESSAGEMODEL_H

#include "libruqola_private_export.h"
#include "messagemodel.h"

class LIBRUQOLACORE_TESTS_EXPORT ThreadMessageModel : public MessageModel
{
    Q_OBJECT
public:
    explicit ThreadMessageModel(const QString &roomID = QStringLiteral("no_room"), RocketChatAccount *account = nullptr, Room *room = nullptr, QObject *parent = nullptr);
    ~ThreadMessageModel() override;

    Q_REQUIRED_RESULT QString threadMessageId() const;
    void setThreadMessageId(const QString &threadMessageId);

    void parseThreadMessages(const QJsonObject &obj);
    void loadMoreThreadMessages(const QJsonObject &obj);

    Q_REQUIRED_RESULT int total() const;
    void setTotal(int total);

private:
    void parse(const QJsonObject &obj);
    QString mThreadMessageId;
    int mTotal = 0;
};

#endif // THREADMESSAGEMODEL_H
