/*
   SPDX-FileCopyrightText: 2024-2026 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "messageurls.h"

#include "messageutils.h"
#include "ruqola_message_memory_debug.h"
#include <QJsonArray>
#include <QJsonObject>

using namespace Qt::Literals::StringLiterals;
MessageUrls::MessageUrls()

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
    for (int i = 0; i < urls.size(); i++) {
        const QJsonObject url = urls.at(i).toObject();
        MessageUrl messageUrl;
        messageUrl.setUrlId(MessageUtils::generateUniqueId(messageId, i));
        messageUrl.parseUrl(url);
        if (!messageUrl.isEmpty()) {
            mMessageUrls.append(std::move(messageUrl));
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

QJsonArray MessageUrls::serialize(const MessageUrls &urls)
{
    QJsonArray urlArray;
    for (const auto &urlInfo : urls.messageUrls()) {
        urlArray.append(MessageUrl::serialize(urlInfo));
    }
    return urlArray;
}

MessageUrls *MessageUrls::deserialize(const QJsonArray &urlsArray, const QByteArray &messageId)
{
    QList<MessageUrl> urls;
    for (int i = 0; i < urlsArray.count(); ++i) {
        const QJsonObject urlObj = urlsArray.at(i).toObject();
        MessageUrl url = MessageUrl::deserialize(urlObj);
        url.setUrlId(MessageUtils::generateUniqueId(messageId, i));
        if (!url.isEmpty()) {
            urls.append(std::move(url));
        }
    }

    auto final = new MessageUrls;
    final->setMessageUrls(urls);
    return final;
}

bool MessageUrls::isEmpty() const
{
    return mMessageUrls.isEmpty();
}
