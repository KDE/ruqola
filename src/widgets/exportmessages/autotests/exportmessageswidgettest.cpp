/*
   SPDX-FileCopyrightText: 2020-2023 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "exportmessageswidgettest.h"
#include "exportmessages/exportmessageswidget.h"
#include <QComboBox>
#include <QDateEdit>
#include <QFormLayout>
#include <QTest>
QTEST_MAIN(ExportMessagesWidgetTest)
ExportMessagesWidgetTest::ExportMessagesWidgetTest(QObject *parent)
    : QObject(parent)
{
}

void ExportMessagesWidgetTest::shouldHaveDefaultValues()
{
    ExportMessagesWidget w;
    auto mainLayout = w.findChild<QFormLayout *>(QStringLiteral("mainLayout"));
    QVERIFY(mainLayout);

    auto mFromDate = w.findChild<QDateEdit *>(QStringLiteral("mFromDate"));
    QVERIFY(mFromDate);

    auto mToDate = w.findChild<QDateEdit *>(QStringLiteral("mToDate"));
    QVERIFY(mToDate);

    auto mFormat = w.findChild<QComboBox *>(QStringLiteral("mFormat"));
    QVERIFY(mFormat);

    QVERIFY(!w.roomExportInfo().isValid());
}
