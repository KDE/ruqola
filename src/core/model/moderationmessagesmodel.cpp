/*
   SPDX-FileCopyrightText: 2023-2026 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "moderationmessagesmodel.h"
using namespace Qt::Literals::StringLiterals;

#include "moderation/moderationlistmessages.h"

ModerationMessagesModel::ModerationMessagesModel(RocketChatAccount *account, QObject *parent)
    : CommonMessagesModel{account, parent}
{
}

ModerationMessagesModel::~ModerationMessagesModel() = default;

QList<Message> ModerationMessagesModel::extractMessages(const QJsonObject &obj)
{
    ModerationListMessages messages;
    messages.parseMessages(obj, u"messages"_s);
    return messages.listMessages();
}

#include "moc_moderationmessagesmodel.cpp"
