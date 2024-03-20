/*
   SPDX-FileCopyrightText: 2022-2024 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#pragma once
#include "config-ruqola.h"
#include <KLocalizedString>
#include <QCommandLineParser>

static void ruqolaOptions(QCommandLineParser *parser)
{
    parser->addOption(QCommandLineOption(QStringList() << QStringLiteral("list-accounts"), i18n("Return lists of accounts")));
    parser->addOption(QCommandLineOption(QStringList() << QStringLiteral("account"), i18n("Start with specific account"), QStringLiteral("accountname")));
    parser->addOption(QCommandLineOption(QStringList() << QStringLiteral("messageurl"), i18n("Show Message"), QStringLiteral("url")));
#if USE_RESTAPI_LOGIN_CMAKE_SUPPORT
    // Laurent don't translate it's temporary otherwise I will change name
    parser->addOption(QCommandLineOption(QStringList() << QStringLiteral("loginrestapi"), QStringLiteral("Use restapi for login")));
#endif

#if HAVE_KUSERFEEDBACK
    parser->addOption(QCommandLineOption(QStringLiteral("feedback"), i18n("Lists the available options for user feedback")));
#endif
}
