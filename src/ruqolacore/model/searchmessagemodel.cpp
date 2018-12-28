/*
   Copyright (c) 2018-2019 Montel Laurent <montel@kde.org>

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

#include <QJsonArray>

SearchMessageModel::SearchMessageModel(QObject *parent)
    : QAbstractListModel(parent)
{
}

SearchMessageModel::~SearchMessageModel()
{
}

QVariant SearchMessageModel::data(const QModelIndex &index, int role) const
{
    if (index.row() < 0 || index.row() >= mSearchMessages.count()) {
        return {};
    }
    const SearchMessage message = mSearchMessages.at(index.row());
    switch (role) {
    case MessageConvertedText:
        return message.text();
    case Timestamp:
        return message.timeStamp();
    case MessageId:
        //TODO ?
        return {};
    }

    return {};
}

QHash<int, QByteArray> SearchMessageModel::roleNames() const
{
    QHash<int, QByteArray> roles;
    roles[SearchMessageModel::MessageConvertedText] = QByteArrayLiteral("messagetext");
    roles[SearchMessageModel::Timestamp] = QByteArrayLiteral("timestamp");
    roles[SearchMessageModel::MessageId] = QByteArrayLiteral("messageid");
    //Add more ????
    return roles;
}

int SearchMessageModel::rowCount(const QModelIndex &parent) const
{
    Q_UNUSED(parent);
    return mSearchMessages.count();
}

void SearchMessageModel::parseResult(const QJsonObject &obj, bool restApi)
{
    QVector<SearchMessage> messages;
    const QJsonArray messagesObj = obj.value(QLatin1String("messages")).toArray();
    for (int i = 0; i < messagesObj.size(); i++) {
        const QJsonObject o = messagesObj.at(i).toObject();
        SearchMessage msg;
        msg.parseResult(o, restApi);
        //Verify that it's valid
        messages.append(msg);
    }
    setMessages(messages);
}

void SearchMessageModel::clear()
{
    if (!mSearchMessages.isEmpty()) {
        beginRemoveRows(QModelIndex(), 0, mSearchMessages.count() - 1);
        mSearchMessages.clear();
        endRemoveRows();
    }
}

void SearchMessageModel::setMessages(const QVector<SearchMessage> &messages)
{
    if (rowCount() != 0) {
        beginRemoveRows(QModelIndex(), 0, mSearchMessages.count() - 1);
        mSearchMessages.clear();
        endRemoveRows();
    }
    if (!messages.isEmpty()) {
        beginInsertRows(QModelIndex(), 0, messages.count() - 1);
        mSearchMessages = messages;
        endInsertRows();
    }
}
