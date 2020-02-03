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

#include "searchmessagemodel.h"
#include "ruqola_debug.h"
#include "rocketchataccount.h"
#include "textconverter.h"

#include <QDateTime>
#include <QJsonArray>

#include "listmessages.h"

SearchMessageModel::SearchMessageModel(const QString &roomID, RocketChatAccount *account, Room *room, QObject *parent)
    : MessageModel(roomID, account, room, parent)
{
}

SearchMessageModel::~SearchMessageModel()
{
}

void SearchMessageModel::parse(const QJsonObject &obj)
{
    clear();
    ListMessages messages;
    messages.parseMessages(obj, QStringLiteral("messages"));
    mTotal = messages.total();
    for (int i = 0, total = messages.count(); i < total; ++i) {
        addMessage(messages.at(i));
    }
    setStringNotFound(rowCount() == 0);
}

void SearchMessageModel::setStringNotFound(bool stringNotFound)
{
    if (mStringNotFound != stringNotFound) {
        mStringNotFound = stringNotFound;
        Q_EMIT stringNotFoundChanged();
    }
}

bool SearchMessageModel::stringNotFound() const
{
    return mStringNotFound;
}
