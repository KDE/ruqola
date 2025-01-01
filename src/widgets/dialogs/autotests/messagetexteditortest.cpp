/*
   SPDX-FileCopyrightText: 2021-2025 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "messagetexteditortest.h"
#include "dialogs/messagetexteditor.h"
#include <QTest>
QTEST_MAIN(MessageTextEditorTest)
MessageTextEditorTest::MessageTextEditorTest(QObject *parent)
    : QObject(parent)
{
}

void MessageTextEditorTest::shouldHaveDefaultValues()
{
    MessageTextEditor d;
    QVERIFY(!d.acceptRichText());
    QVERIFY(d.toPlainText().isEmpty());
    QVERIFY(d.tabChangesFocus());
}

#include "moc_messagetexteditortest.cpp"
