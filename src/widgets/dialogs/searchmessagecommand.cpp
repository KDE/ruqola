/*
   SPDX-FileCopyrightText: 2026 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/
#include "searchmessagecommand.h"

using namespace Qt::Literals::StringLiterals;
SearchMessageCommand::SearchMessageCommand() = default;

SearchMessageCommand::~SearchMessageCommand() = default;

QString SearchMessageCommand::generateCommandText(SearchMessageCommand::SearchMessageCommandType type)
{
    switch (type) {
    case Unknown:
        return {};
    case HasStar:
        return u"has:star"_s;
    case IsPinned:
        return u"is:pinned"_s;
    case HasUrl:
        return u"has:url"_s;
    case HasLocation:
        return u"has:location"_s;
    case Before:
        return u"before:"_s;
    case After:
        return u"after:"_s;
    case Day:
        return u"on:"_s;
    case Order:
        return u"order:desc"_s;
    case FromMe:
        return u"from:me"_s;
    case FromUserName:
        return u"from:user.name"_s;
    }
    return {};
}

bool SearchMessageCommand::mustBeUnique(SearchMessageCommand::SearchMessageCommandType type)
{
    return type == HasStar || type == IsPinned || type == HasUrl || type == HasLocation || type == Order || type == FromMe || type == FromUserName;
}

#include "moc_searchmessagecommand.cpp"
