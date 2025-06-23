/*
   SPDX-FileCopyrightText: 2023-2025 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/
#include "moderationlistmessages.h"

#include "moderationmessage.h"
#include "ruqola_debug.h"

using namespace Qt::Literals::StringLiterals;
ModerationListMessages::ModerationListMessages() = default;

ModerationListMessages::~ModerationListMessages() = default;

void ModerationListMessages::parseMessagesList(const QJsonObject &messagesObj, const QString &arrayName)
{
    const QJsonArray messagesArray = messagesObj[arrayName.isEmpty() ? u"messages"_s : arrayName].toArray();
    mListMessages.reserve(messagesArray.count());
    for (const QJsonValue &current : messagesArray) {
        if (current.type() == QJsonValue::Object) {
            const QJsonObject messageModerationObject = current.toObject();
            Message m;
            ModerationMessage moderationMessage;
            moderationMessage.parse(messageModerationObject);
            m.parseMessage(messageModerationObject["message"_L1].toObject(), true, nullptr);
            m.setModerationMessage(moderationMessage);
            mListMessages.append(std::move(m));
        } else {
            qCWarning(RUQOLA_LOG) << "Problem when parsing moderation message" << current;
        }
    }
}
