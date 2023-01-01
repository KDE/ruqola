/*
   SPDX-FileCopyrightText: 2022-2023 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "channelnamevalidlinewidgettest.h"
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
    auto mainLayout = w.findChild<QVBoxLayout *>(QStringLiteral("mainLayout"));
    QVERIFY(mainLayout);
    QCOMPARE(mainLayout->contentsMargins(), QMargins{});
    QVERIFY(w.text().isEmpty());

    auto mChannelNameValidLineEdit = w.findChild<ChannelNameValidLineEdit *>(QStringLiteral("mChannelNameValidLineEdit"));
    QVERIFY(mChannelNameValidLineEdit);

    auto mChannelNameLabel = w.findChild<QLabel *>(QStringLiteral("mChannelNameLabel"));
    QCOMPARE(mChannelNameLabel->textFormat(), Qt::PlainText);
}
