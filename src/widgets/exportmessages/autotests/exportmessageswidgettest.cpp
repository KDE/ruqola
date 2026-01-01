/*
   SPDX-FileCopyrightText: 2020-2026 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "exportmessageswidgettest.h"
using namespace Qt::Literals::StringLiterals;

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
    auto mainLayout = w.findChild<QFormLayout *>(u"mainLayout"_s);
    QVERIFY(mainLayout);

    auto mFromDate = w.findChild<QDateEdit *>(u"mFromDate"_s);
    QVERIFY(mFromDate);

    auto mToDate = w.findChild<QDateEdit *>(u"mToDate"_s);
    QVERIFY(mToDate);

    auto mFormat = w.findChild<QComboBox *>(u"mFormat"_s);
    QVERIFY(mFormat);

    QVERIFY(!w.roomExportInfo().isValid());
}

#include "moc_exportmessageswidgettest.cpp"
