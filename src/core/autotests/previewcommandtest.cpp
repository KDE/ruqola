/*
   SPDX-FileCopyrightText: 2025 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/
#include "previewcommandtest.h"
#include "commands/previewcommand.h"
#include <QTest>
QTEST_GUILESS_MAIN(PreviewCommandTest)

PreviewCommandTest::PreviewCommandTest(QObject *parent)
    : QObject{parent}
{
}

void PreviewCommandTest::shouldHaveDefaultValues()
{
    PreviewCommand c;
    QVERIFY(c.id().isEmpty());
    QVERIFY(c.type().isEmpty());
    QVERIFY(c.value().isEmpty());
}

#include "moc_previewcommandtest.cpp"
