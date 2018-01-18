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
#include "utils.h"
#include "rocketchataccount.h"
#include "texthighlighter.h"
#include "textconverter.h"
#include <KSyntaxHighlighting/Definition>
#include <KSyntaxHighlighting/Repository>
#include <KSyntaxHighlighting/Theme>

#define STORE_MESSAGE 1

MessageModel::MessageModel(const QString &roomID, RocketChatAccount *account, QObject *parent)
    : QAbstractListModel(parent)
    , mRoomID(roomID)
    , mRocketChatAccount(account)
{
    mTextConverter = new TextConverter;
    qCDebug(RUQOLA_LOG) << "Creating message Model";
#ifdef STORE_MESSAGE
    if (mRocketChatAccount) {
        const QString cachePath = mRocketChatAccount->settings()->cacheBasePath();
        if (cachePath.isEmpty()) {
            qCWarning(RUQOLA_LOG) << " Cache Path is not defined";
            return;
        }

        QDir cacheDir(cachePath + QStringLiteral("/rooms_cache"));

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
#endif
}

MessageModel::~MessageModel()
{
#ifdef STORE_MESSAGE
    if (mRocketChatAccount) {
        const QString cachePath = mRocketChatAccount->settings()->cacheBasePath();
        if (cachePath.isEmpty()) {
            qCWarning(RUQOLA_LOG) << " Cache Path is not defined";
            return;
        }

        QDir cacheDir(cachePath + QStringLiteral("/rooms_cache"));
        qCDebug(RUQOLA_LOG) << "Caching to..." << cacheDir.path();
        if (!cacheDir.exists(cacheDir.path())) {
            cacheDir.mkpath(cacheDir.path());
        }

        QFile f(cacheDir.absoluteFilePath(mRoomID));

        if (f.open(QIODevice::WriteOnly)) {
            QDataStream out(&f);
            for (const Message &m : qAsConst(mAllMessages)) {
                const QByteArray ms = Message::serialize(m);
                out.writeBytes(ms, ms.size());
            }
        }
    }
#endif
    delete mTextConverter;
}

QHash<int, QByteArray> MessageModel::roleNames() const
{
    QHash<int, QByteArray> roles;
    roles[MessageText] = "messageText";
    roles[Username] = "username";
    roles[Timestamp] = "timestamp";
    roles[UserId] = "userID";
    roles[SystemMessageType] = "type";
    roles[MessageId] = "messageID";
    roles[RoomId] = "roomID";
    roles[UpdatedAt] = "updatedAt";
    roles[EditedAt] = "editedAt";
    roles[EditedByUserName] = "editedByUsername";
    roles[EditedByUserId] = "editedByUserID";
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
    if (!mAllMessages.isEmpty()) {
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
    auto it = std::upper_bound(mAllMessages.begin(), mAllMessages.end(), message,
                               [](const Message &lhs, const Message &rhs) -> bool {
        return lhs.timeStamp() < rhs.timeStamp();
    }
    );
    //When we have 1 element.
    if (mAllMessages.count() == 1 && (*mAllMessages.begin()).messageId() == message.messageId()) {
        (*mAllMessages.begin()) = message;
        const QModelIndex index = createIndex(0, 0);
        qCDebug(RUQOLA_LOG) << "Update Message";
        Q_EMIT dataChanged(index, index);
    } else if (((it) != mAllMessages.begin() && (*(it - 1)).messageId() == message.messageId())) {
        qCDebug(RUQOLA_LOG) << "Update Message";
        (*(it-1)) = message;
        const QModelIndex index = createIndex(it - 1 - mAllMessages.begin(), 0);
        Q_EMIT dataChanged(index, index);
    } else {
        const int pos = it - mAllMessages.begin();
        beginInsertRows(QModelIndex(), pos, pos);
        mAllMessages.insert(it, message);
        endInsertRows();
    }
}

QVariant MessageModel::data(const QModelIndex &index, int role) const
{
    if (!index.isValid()) {
        qCWarning(RUQOLA_LOG) << "ERROR: invalid index";
        return {};
    }
    int idx = index.row();
    switch (role) {
    case MessageModel::Username:
        return mAllMessages.at(idx).username();
    case MessageModel::MessageText:
        return convertMessageText(mAllMessages.at(idx).text(), mAllMessages.at(idx).mentions());
    case MessageModel::Timestamp:
        return mAllMessages.at(idx).timeStamp();
    case MessageModel::UserId:
        return mAllMessages.at(idx).userId();
    case MessageModel::SystemMessageType:
        return mAllMessages.at(idx).systemMessageType();
    case MessageModel::MessageId:
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
    case MessageModel::Attachments:
    {
        QVariantList lst;
        lst.reserve(mAllMessages.at(idx).attachements().count());
        for (const MessageAttachment &att : mAllMessages.at(idx).attachements()) {
            lst.append(QVariant::fromValue(att));
        }
        return lst;
    }
    case MessageModel::Urls:
    {
        QVariantList lst;
        lst.reserve(mAllMessages.at(idx).urls().count());
        for (const MessageUrl &url : mAllMessages.at(idx).urls()) {
            lst.append(QVariant::fromValue(url));
        }
        return lst;
    }
    case MessageModel::Date:
        if (idx > 0) {
            QDateTime previewDate;
            previewDate.setMSecsSinceEpoch(mAllMessages.at(idx - 1).timeStamp());
            QDateTime currentDate;
            currentDate.setMSecsSinceEpoch(mAllMessages.at(idx).timeStamp());
            if (previewDate.date() != currentDate.date()) {
                return currentDate.date().toString();
            }
        }
        return QString();
    default:
        return QString();
    }
}

QString MessageModel::convertMessageText(const QString &str, const QMap<QString, QString> &mentions) const
{
    return mTextConverter->convertMessageText(str, mentions);
}

void MessageModel::deleteMessage(const QString &messageId)
{
    for (int i = 0; i < mAllMessages.count(); ++i) {
        if (mAllMessages.at(i).messageId() == messageId) {
            beginRemoveRows(QModelIndex(), i, i);
            mAllMessages.remove(i);
            endRemoveRows();
            break;
        }
    }
}


