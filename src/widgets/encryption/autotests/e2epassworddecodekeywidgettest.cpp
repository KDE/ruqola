/*
  SPDX-FileCopyrightText: 2024-2026 Laurent Montel <montel@kde.org>

  SPDX-License-Identifier: GPL-2.0-or-later
*/

#include "e2epassworddecodekeywidgettest.h"

#include "encryption/e2epassworddecodekeywidget.h"
#include <KPasswordLineEdit>
#include <QLabel>
#include <QSignalSpy>
#include <QTest>
#include <QVBoxLayout>
#include <qsignalspy.h>
#include <qtestcase.h>

using namespace Qt::Literals::StringLiterals;

QTEST_MAIN(E2ePasswordDecodeKeyWidgetTest)

E2ePasswordDecodeKeyWidgetTest::E2ePasswordDecodeKeyWidgetTest(QObject *parent)
    : QObject{parent}
{
}

void E2ePasswordDecodeKeyWidgetTest::shouldHaveDefaultValues()
{
    const E2ePasswordDecodeKeyWidget w;

    auto mainLayout = w.findChild<QVBoxLayout *>(u"mainLayout"_s);
    QVERIFY(mainLayout);
    QCOMPARE(mainLayout->contentsMargins(), QMargins{});

    auto label = w.findChild<QLabel *>(u"label"_s);
    QVERIFY(label);
    QVERIFY(!label->text().isEmpty());
    QVERIFY(label->wordWrap());

    auto mPassword = w.findChild<KPasswordLineEdit *>(u"mPassword"_s);
    QVERIFY(mPassword);
    QVERIFY(mPassword->password().isEmpty());
}

void E2ePasswordDecodeKeyWidgetTest::shouldEmitUpdateButtonSignal()
{
    const E2ePasswordDecodeKeyWidget w;
    auto mPassword = w.findChild<KPasswordLineEdit *>(u"mPassword"_s);
    QSignalSpy updateButtonStateSpy(&w, &E2ePasswordDecodeKeyWidget::updateButton);

    mPassword->setPassword(u"foo"_s);
    QCOMPARE(updateButtonStateSpy.count(), 1);
    QVERIFY(updateButtonStateSpy.at(0).at(0).toBool());

    updateButtonStateSpy.clear();
    mPassword->setPassword({});
    QCOMPARE(updateButtonStateSpy.count(), 1);
    QVERIFY(!updateButtonStateSpy.at(0).at(0).toBool());
}

#include "moc_e2epassworddecodekeywidgettest.cpp"
