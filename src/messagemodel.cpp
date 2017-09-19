/*

 * Copyright 2016  Riccardo Iaconelli <riccardo@kde.org>
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License as
 * published by the Free Software Foundation; either version 2 of
 * the License or (at your option) version 3 or any later version
 * accepted by the membership of KDE e.V. (or its successor approved
 * by the membership of KDE e.V.), which shall act as a proxy
 * defined in Section 14 of version 3 of the license.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 *
 */

#include <QFile>
#include <QVector>
#include <QJsonObject>
#include <QJsonDocument>
#include <QDataStream>
#include <QModelIndex>
#include <QAbstractListModel>

#include "messagemodel.h"
#include "message.h"
#include "ruqola_debug.h"
#include "rocketchataccount.h"

MessageModel::MessageModel(const QString &roomID, RocketChatAccount *account, QObject *parent)
    : QAbstractListModel(parent)
    , m_roomID(roomID)
    , mRocketChatAccount(account)
{
    qCDebug(RUQOLA_LOG) << "Creating message Model";
    if (mRocketChatAccount) {
        QDir cacheDir(mRocketChatAccount->settings()->cacheBasePath()+QStringLiteral("/rooms_cache"));

        // load cache
        if (QFile::exists(cacheDir.absoluteFilePath(roomID)) && !roomID.isEmpty()) {
            QFile f(cacheDir.absoluteFilePath(roomID));
            if (f.open(QIODevice::ReadOnly)) {
                QDataStream in(&f);
                while (!f.atEnd()) {
                    char *byteArray;
                    quint32 length;
                    in.readBytes(byteArray, length);
                    const QByteArray arr = QByteArray::fromRawData(byteArray, length);
                    Message m = Message::fromJSon(QJsonDocument::fromBinaryData(arr).object());
                    addMessage(m);
                }
            }
        }
    }
}

MessageModel::~MessageModel()
{
    if (mRocketChatAccount) {
        QDir cacheDir(mRocketChatAccount->settings()->cacheBasePath()+QStringLiteral("/rooms_cache"));
        qCDebug(RUQOLA_LOG) << "Caching to..." << cacheDir.path();
        if (!cacheDir.exists(cacheDir.path())) {
            cacheDir.mkpath(cacheDir.path());
        }

        QFile f(cacheDir.absoluteFilePath(m_roomID));

        if (f.open(QIODevice::WriteOnly)) {
            QDataStream out(&f);
            for (const Message &m : qAsConst(mAllMessages)) {
                const QByteArray ms = Message::serialize(m);
                out.writeBytes(ms, ms.size());
            }
        }
    }
}

QHash<int, QByteArray> MessageModel::roleNames() const
{
    QHash<int, QByteArray> roles;
    roles[MessageText] = "messageText";
    roles[Username] = "username";
    roles[Timestamp] = "timestamp";
    roles[UserID] = "userID";
    roles[SystemMessageType] = "type";
    roles[MessageID] = "messageID";
    roles[RoomID] = "roomID";
    roles[UpdatedAt] = "updatedAt";
    roles[EditedAt] = "editedAt";
    roles[EditedByUserName] = "editedByUsername";
    roles[EditedByUserID] = "editedByUserID";
    roles[Alias] = "alias";
    roles[Avatar] = "avatar";
    roles[Groupable] = "groupable";
    roles[MessageType] = "messagetype";
    roles[Attachments] = "attachments";
    roles[Urls] = "urls";
    roles[Date] = "date";

    return roles;
}

qint64 MessageModel::lastTimestamp() const
{
    if (mAllMessages.size()) {
        qCDebug(RUQOLA_LOG) << "returning timestamp" << mAllMessages.last().timeStamp();
        return mAllMessages.last().timeStamp();
    } else {
        return 0;
    }
}

int MessageModel::rowCount(const QModelIndex &parent) const
{
    Q_UNUSED(parent);
    return mAllMessages.size();
}

void MessageModel::addMessage(const Message &message)
{
#if 0
    // Don't add empty messages
    if (message.text().isEmpty()) {
        return;
    }
#endif

    auto existingMessage = std::find(mAllMessages.begin(), mAllMessages.end(), message);
    bool present = (existingMessage != mAllMessages.end());
    auto i = std::upper_bound(mAllMessages.begin(), mAllMessages.end(), message);
    int pos = i-mAllMessages.begin();
    bool messageChanged = false;

    if (present) {
        messageChanged = true;
        //Figure out a better way to update just the really changed message
    } else {
        beginInsertRows(QModelIndex(), pos, pos);
    }

    if (messageChanged) {
        mAllMessages.replace(pos-1, message);
    } else {
        mAllMessages.insert(i, message);
    }

    if (messageChanged) {
        Q_EMIT dataChanged(createIndex(1, 1), createIndex(pos, 1));
    } else {
        endInsertRows();
    }
}

QVariant MessageModel::data(const QModelIndex &index, int role) const
{
    int idx = index.row();
    switch (role) {
    case MessageModel::Username:
        return mAllMessages.at(idx).username();
    case MessageModel::MessageText:
        return mAllMessages.at(idx).text();
    case MessageModel::Timestamp:
        return mAllMessages.at(idx).timeStamp();
    case MessageModel::UserID:
        return mAllMessages.at(idx).userId();
    case MessageModel::SystemMessageType:
        return mAllMessages.at(idx).systemMessageType();
    case MessageModel::MessageID:
        return mAllMessages.at(idx).messageId();
    case MessageModel::Alias:
        return mAllMessages.at(idx).alias();
    case MessageModel::MessageType:
        return mAllMessages.at(idx).messageType();
    case MessageModel::Avatar:
        return mAllMessages.at(idx).avatar();
    case MessageModel::EditedAt:
        return mAllMessages.at(idx).editedAt();
    case MessageModel::EditedByUserName:
        return mAllMessages.at(idx).editedByUsername();
    case MessageModel::Attachments: {
        QVariantList lst;
        for (const MessageAttachment &att : mAllMessages.at(idx).attachements()) {
            lst.append(QVariant::fromValue(att));
        }
        if (!lst.isEmpty()) {
            qDebug() << " AAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAaaa"<<lst;
        }
        return lst;
    }
    case MessageModel::Urls: {
        QVariantList lst;
        for (const MessageUrl &url : mAllMessages.at(idx).urls()) {
            lst.append(QVariant::fromValue(url));
        }
        return lst;
    }
    case MessageModel::Date: {
        if (idx > 0) {
            QDateTime previewDate;
            previewDate.setMSecsSinceEpoch(mAllMessages.at(idx - 1).timeStamp());
            QDateTime currentDate;
            currentDate.setMSecsSinceEpoch(mAllMessages.at(idx).timeStamp());
            if (previewDate.date() != currentDate.date()) {
                return currentDate.toString();
            }
        }
        return QString();
    }
    default:
        return QString();
    }    
}
