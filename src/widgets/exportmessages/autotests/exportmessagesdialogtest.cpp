/*
   SPDX-FileCopyrightText: 2020-2024 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "exportmessagesdialogtest.h"
#include "exportmessages/exportmessagesdialog.h"
#include "exportmessages/exportmessageswidget.h"
#include <QDialogButtonBox>
#include <QStandardPaths>
#include <QTest>
#include <QVBoxLayout>
QTEST_MAIN(ExportMessagesDialogTest)

ExportMessagesDialogTest::ExportMessagesDialogTest(QObject *parent)
    : QObject(parent)
{
    QStandardPaths::setTestModeEnabled(true);
}

void ExportMessagesDialogTest::shouldHaveDefaultValues()
{
    ExportMessagesDialog d;
    QVERIFY(!d.windowTitle().isEmpty());

    auto mainLayout = d.findChild<QVBoxLayout *>(QStringLiteral("mainLayout"));
    QVERIFY(mainLayout);

    auto mExportMessagesWidget = d.findChild<ExportMessagesWidget *>(QStringLiteral("mExportMessagesWidget"));
    QVERIFY(mExportMessagesWidget);

    auto button = d.findChild<QDialogButtonBox *>(QStringLiteral("button"));
    QVERIFY(button);

    QVERIFY(!d.roomExportInfo().isValid());
}

#include "moc_exportmessagesdialogtest.cpp"
