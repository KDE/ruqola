/*
   SPDX-FileCopyrightText: 2023-2026 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/
#include "moderationlistmessages.h"

#include "moderationmessage.h"
#include "ruqola_debug.h"

using namespace Qt::Literals::StringLiterals;
ModerationListMessages::ModerationListMessages() = default;

ModerationListMessages::~ModerationListMessages() = default;

void ModerationListMessages::parseMessagesList(const QJsonObject &messagesObj, QLatin1StringView arrayName)
{
    const QJsonArray messagesArray = messagesObj[arrayName].toArray();
    mList.reserve(mList.count() + messagesArray.count());
    for (const QJsonValue &current : messagesArray) {
        if (current.type() == QJsonValue::Object) {
            const QJsonObject messageModerationObject = current.toObject();
            ModerationMessage moderationMessage;
            moderationMessage.parse(messageModerationObject);
            Message &m = mList.emplace_back();
            m.parseMessage(messageModerationObject["message"_L1].toObject(), true, nullptr);
            m.setModerationMessage(moderationMessage);
        } else {
            qCWarning(RUQOLA_LOG) << "Problem when parsing moderation message" << current;
        }
    }
}
