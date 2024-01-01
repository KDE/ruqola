/*
   SPDX-FileCopyrightText: 2020-2024 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "channelpasswordwidgettest.h"
#include "dialogs/channelpasswordwidget.h"
#include <KPasswordLineEdit>
#include <QHBoxLayout>
#include <QLabel>
#include <QSignalSpy>
#include <QTest>
QTEST_MAIN(ChannelPasswordWidgetTest)
ChannelPasswordWidgetTest::ChannelPasswordWidgetTest(QObject *parent)
    : QObject(parent)
{
}

void ChannelPasswordWidgetTest::shouldHaveDefaultValues()
{
    ChannelPasswordWidget w;
    auto mainLayout = w.findChild<QHBoxLayout *>(QStringLiteral("mainLayout"));
    QVERIFY(mainLayout);
    QCOMPARE(mainLayout->contentsMargins(), QMargins{});

    auto label = w.findChild<QLabel *>(QStringLiteral("label"));
    QVERIFY(label);
    QVERIFY(!label->text().isEmpty());

    auto mPasswordLineEdit = w.findChild<KPasswordLineEdit *>(QStringLiteral("mPasswordLineEdit"));
    QVERIFY(mPasswordLineEdit);

    QVERIFY(w.password().isEmpty());
}

void ChannelPasswordWidgetTest::shouldEmitSignals()
{
    ChannelPasswordWidget w;
    auto mPasswordLineEdit = w.findChild<KPasswordLineEdit *>(QStringLiteral("mPasswordLineEdit"));
    QSignalSpy spy(&w, &ChannelPasswordWidget::updateOkButton);
    mPasswordLineEdit->setPassword(QStringLiteral("bla"));
    QVERIFY(spy.count() > 0);
    QVERIFY(spy.at(0).at(0).toBool());
    spy.clear();
    mPasswordLineEdit->setPassword({});
    QVERIFY(spy.count() > 0);
    QVERIFY(!spy.at(0).at(0).toBool());
}

#include "moc_channelpasswordwidgettest.cpp"
