/*
   Copyright (c) 2018-2020 Laurent Montel <montel@kde.org>

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

#ifndef SEARCHMESSAGEMODEL_H
#define SEARCHMESSAGEMODEL_H

#include <QAbstractListModel>
#include <QJsonObject>
#include "libruqola_private_export.h"
#include "messagemodel.h"
class RocketChatAccount;
class LIBRUQOLACORE_TESTS_EXPORT SearchMessageModel : public MessageModel
{
    Q_OBJECT
public:
    explicit SearchMessageModel(const QString &roomID = QStringLiteral("no_room"), RocketChatAccount *account = nullptr, Room *room = nullptr, QObject *parent = nullptr);
    ~SearchMessageModel() override;
    void parse(const QJsonObject &obj);

    Q_REQUIRED_RESULT bool stringNotFound() const;
Q_SIGNALS:
    void listMessageTypeChanged();
    void stringNotFoundChanged();
private:
    void checkFullList();
    void setStringNotFound(bool stringNotFound);
    QString mRoomId;
    int mTotal = 0;
    bool mStringNotFound = false;
};

#endif // SEARCHMESSAGEMODEL_H
