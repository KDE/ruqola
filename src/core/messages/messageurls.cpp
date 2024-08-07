/*
   SPDX-FileCopyrightText: 2024 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "messageurls.h"

#include "ruqola_message_memory_debug.h"
#include <QJsonArray>
#include <QJsonObject>

using namespace Qt::Literals::StringLiterals;
MessageUrls::MessageUrls()
    : QSharedData()
{
    qCDebug(RUQOLA_MESSAGE_MEMORY_LOG) << " MessageUrls created " << this;
}

MessageUrls::MessageUrls(const MessageUrls &other)
    : QSharedData(other)
{
    qCDebug(RUQOLA_MESSAGE_MEMORY_LOG) << " MessageUrls(const MessageUrls &other) created " << this;
    mMessageUrls = other.messageUrls();
}

MessageUrls::~MessageUrls()
{
    qCDebug(RUQOLA_MESSAGE_MEMORY_LOG) << " MessageUrls deleted " << this;
}

void MessageUrls::setMessageUrls(const QList<MessageUrl> &messageUrls)
{
    mMessageUrls = messageUrls;
}

QList<MessageUrl> MessageUrls::messageUrls() const
{
    return mMessageUrls;
}

void MessageUrls::parseMessageUrls(const QJsonArray &urls, const QByteArray &messageId)
{
    mMessageUrls.clear();
    if (!urls.isEmpty()) {
        for (int i = 0; i < urls.size(); i++) {
            const QJsonObject url = urls.at(i).toObject();
            MessageUrl messageUrl;
            messageUrl.setUrlId(MessageUrls::generateUniqueId(messageId, i));
            messageUrl.parseUrl(url);
            if (!messageUrl.isEmpty()) {
                mMessageUrls.append(messageUrl);
            }
        }
    }
}

bool MessageUrls::operator==(const MessageUrls &other) const
{
    return mMessageUrls == other.messageUrls();
}

QDebug operator<<(QDebug d, const MessageUrls &t)
{
    for (int i = 0; i < t.messageUrls().count(); i++) {
        d.space() << t.messageUrls().at(i) << "\n";
    }
    return d;
}

QJsonObject MessageUrls::serialize(const MessageUrls &blocks)
{
    QJsonObject obj;
    // TODO
    return obj;
}

MessageUrls *MessageUrls::deserialize(const QJsonObject &o)
{
    // TODO
    return {};
}

bool MessageUrls::isEmpty() const
{
    return mMessageUrls.isEmpty();
}

QByteArray MessageUrls::generateUniqueId(const QByteArray &messageId, int index)
{
    return messageId + QByteArray("_") + QByteArray::number(index);
}
