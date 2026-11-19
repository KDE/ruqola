/*
   SPDX-FileCopyrightText: 2026 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/
#include "toolutils.h"

#include <KLocalizedString>
#include <QJsonArray>
#include <QJsonObject>
#include <TextAutoGenerateTextMcpProtocolCore/McpProtocolCommonUtils>
#include <TextAutoGenerateTextMcpProtocolCore/McpProtocolTool>
#include <algorithm>
using namespace Qt::Literals::StringLiterals;
namespace
{
[[nodiscard]] bool starredByUser(const QJsonObject &message, const QString &userId)
{
    const QJsonArray starred = message.value("starred"_L1).toArray();
    return std::any_of(starred.cbegin(), starred.cend(), [&userId](const QJsonValue &value) {
        return value.toObject().value("_id"_L1).toString() == userId;
    });
}
}

QString ToolUtils::groupTitleFromType(TitleType type)
{
    switch (type) {
    case Rooms:
        return i18n("Rooms");
    case Messages:
        return i18n("Messages");
    case Users:
        return i18n("Users");
    case CurrentUser:
        return i18n("Current User");
    case Misc:
        return i18n("Misc");
    }
    Q_UNREACHABLE();
    return {};
}

QString ToolUtils::groupFromType(TitleType type)
{
    switch (type) {
    case Rooms:
        return u"rooms"_s;
    case Messages:
        return u"messages"_s;
    case Users:
        return u"users"_s;
    case CurrentUser:
        return u"currentuser"_s;
    case Misc:
        return u"misc"_s;
    }
    Q_UNREACHABLE();
    return {};
}

void ToolUtils::addToolMeta(TextAutoGenerateTextMcpProtocolCore::McpProtocolTool &tool, TitleType type)
{
    TextAutoGenerateTextMcpProtocolCore::McpProtocolCommonUtils::addToolMeta(tool, groupFromType(type), groupTitleFromType(type));
}

QString ToolUtils::messageEntry(const QJsonObject &message, const QString &userId)
{
    const QString username = message.value("u"_L1).toObject().value("username"_L1).toString();
    QString entry = u"id: %1, ts: %2, user: %3"_s.arg(message.value("_id"_L1).toString(), message.value("ts"_L1).toString(), username);
    if (const QString threadId = message.value("tmid"_L1).toString(); !threadId.isEmpty()) {
        entry += u", thread: %1"_s.arg(threadId);
    }
    if (message.value("pinned"_L1).toBool()) {
        entry += u" [pinned]";
    }
    if (starredByUser(message, userId)) {
        entry += u" [starred]";
    }
    if (message.value("t"_L1).toString() == "e2e"_L1) {
        entry += u" [encrypted]";
    } else {
        entry += u"\n  text: %1"_s.arg(message.value("msg"_L1).toString());
    }
    return entry;
}
