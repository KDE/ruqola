/*
   SPDX-FileCopyrightText: 2025-2026 Laurent Montel <montel@kde.org>

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
    QCOMPARE(c.type(), PreviewCommand::TypePreview::Unknown);
    QVERIFY(c.value().isEmpty());
    QVERIFY(!c.isValid());
}

#include "moc_previewcommandtest.cpp"
