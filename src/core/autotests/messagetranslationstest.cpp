/*
   SPDX-FileCopyrightText: 2019-2026 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "messagetranslationstest.h"
#include "messages/messagetranslations.h"
#include <QTest>
QTEST_GUILESS_MAIN(MessageTranslationsTest)

MessageTranslationsTest::MessageTranslationsTest(QObject *parent)
    : QObject(parent)
{
}

void MessageTranslationsTest::shouldHaveDefaultValues()
{
    MessageTranslations t;
    QVERIFY(t.translatedString().isEmpty());
    QVERIFY(t.isEmpty());
}

#include "moc_messagetranslationstest.cpp"
