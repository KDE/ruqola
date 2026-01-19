/*
   SPDX-FileCopyrightText: 2026 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/
#include "searchmessagecommandtest.h"
#include "dialogs/searchmessagecommand.h"
#include <QTest>
QTEST_GUILESS_MAIN(SearchMessageCommandTest)

SearchMessageCommandTest::SearchMessageCommandTest(QObject *parent)
    : QObject{parent}
{
}

#include "moc_searchmessagecommandtest.cpp"
