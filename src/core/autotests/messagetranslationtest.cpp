/*
   SPDX-FileCopyrightText: 2019-2024 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "messagetranslationtest.h"
#include "messages/messagetranslation.h"
#include <QTest>
QTEST_GUILESS_MAIN(MessageTranslationTest)

MessageTranslationTest::MessageTranslationTest(QObject *parent)
    : QObject(parent)
{
}

void MessageTranslationTest::shouldHaveDefaultValues()
{
    MessageTranslation t;
    QVERIFY(t.translatedString().isEmpty());
    QVERIFY(t.isEmpty());
}

#include "moc_messagetranslationtest.cpp"
