/*
   SPDX-FileCopyrightText: 2022-2025 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "messagemaximumsizewidgettest.h"
using namespace Qt::Literals::StringLiterals;

#include "messagemaximumsizedialog/messagemaximumsizewidget.h"
#include <QFormLayout>
#include <QLineEdit>
#include <QSignalSpy>
#include <QTest>
QTEST_MAIN(MessageMaximumSizeWidgetTest)
MessageMaximumSizeWidgetTest::MessageMaximumSizeWidgetTest(QObject *parent)
    : QObject{parent}
{
}

void MessageMaximumSizeWidgetTest::shouldHaveDefaultValues()
{
    MessageMaximumSizeWidget d;

    auto mainLayout = d.findChild<QFormLayout *>(u"mainLayout"_s);
    QVERIFY(mainLayout);
    QCOMPARE(mainLayout->contentsMargins(), QMargins{});

    auto mFileName = d.findChild<QLineEdit *>(u"mFileName"_s);
    QVERIFY(mFileName);
    QVERIFY(mFileName->text().isEmpty());

    auto mDescription = d.findChild<QLineEdit *>(u"mDescription"_s);
    QVERIFY(mDescription);
    QVERIFY(mDescription->text().isEmpty());

    QVERIFY(d.description().isEmpty());
    QVERIFY(d.fileName().isEmpty());
}

void MessageMaximumSizeWidgetTest::shouldEmitSignal()
{
    MessageMaximumSizeWidget d;
    QSignalSpy spyUpdateButton(&d, &MessageMaximumSizeWidget::updateOkButton);
    auto mFileName = d.findChild<QLineEdit *>(u"mFileName"_s);
    auto mDescription = d.findChild<QLineEdit *>(u"mDescription"_s);
    mFileName->setText(u"foo"_s);
    QCOMPARE(spyUpdateButton.count(), 1);
    QVERIFY(!spyUpdateButton.at(0).at(0).toBool());

    spyUpdateButton.clear();
    mDescription->setText(u"bla"_s);
    QCOMPARE(spyUpdateButton.count(), 1);
    QVERIFY(spyUpdateButton.at(0).at(0).toBool());

    // an empty text
    spyUpdateButton.clear();
    mDescription->setText({});
    QCOMPARE(spyUpdateButton.count(), 1);
    QVERIFY(!spyUpdateButton.at(0).at(0).toBool());

    // a trimmed text empty
    spyUpdateButton.clear();
    mDescription->setText(u" "_s);
    QCOMPARE(spyUpdateButton.count(), 1);
    QVERIFY(!spyUpdateButton.at(0).at(0).toBool());

    spyUpdateButton.clear();
    mFileName->setText(u" "_s);
    QCOMPARE(spyUpdateButton.count(), 1);
    QVERIFY(!spyUpdateButton.at(0).at(0).toBool());
}

#include "moc_messagemaximumsizewidgettest.cpp"
