/*
   SPDX-FileCopyrightText: 2020-2026 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "exportmessagesdialogtest.h"
using namespace Qt::Literals::StringLiterals;

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

    auto mainLayout = d.findChild<QVBoxLayout *>(u"mainLayout"_s);
    QVERIFY(mainLayout);

    auto mExportMessagesWidget = d.findChild<ExportMessagesWidget *>(u"mExportMessagesWidget"_s);
    QVERIFY(mExportMessagesWidget);

    auto button = d.findChild<QDialogButtonBox *>(u"button"_s);
    QVERIFY(button);

    QVERIFY(!d.roomExportInfo().isValid());
}

#include "moc_exportmessagesdialogtest.cpp"
