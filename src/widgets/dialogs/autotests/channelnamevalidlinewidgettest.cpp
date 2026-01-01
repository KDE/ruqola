/*
   SPDX-FileCopyrightText: 2022-2026 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "channelnamevalidlinewidgettest.h"
using namespace Qt::Literals::StringLiterals;

#include "dialogs/channelnamevalidlineedit.h"
#include "dialogs/channelnamevalidlinewidget.h"
#include <QLabel>
#include <QTest>
#include <QVBoxLayout>
QTEST_MAIN(ChannelNameValidLineWidgetTest)
ChannelNameValidLineWidgetTest::ChannelNameValidLineWidgetTest(QObject *parent)
    : QObject{parent}
{
}

void ChannelNameValidLineWidgetTest::shouldHaveDefaultValues()
{
    ChannelNameValidLineWidget w(nullptr);
    auto mainLayout = w.findChild<QVBoxLayout *>(u"mainLayout"_s);
    QVERIFY(mainLayout);
    QCOMPARE(mainLayout->contentsMargins(), QMargins{});
    QVERIFY(w.text().isEmpty());

    auto mChannelNameValidLineEdit = w.findChild<ChannelNameValidLineEdit *>(u"mChannelNameValidLineEdit"_s);
    QVERIFY(mChannelNameValidLineEdit);

    auto mChannelNameLabel = w.findChild<QLabel *>(u"mChannelNameLabel"_s);
    QCOMPARE(mChannelNameLabel->textFormat(), Qt::PlainText);
}

#include "moc_channelnamevalidlinewidgettest.cpp"
