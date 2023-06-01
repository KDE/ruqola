/*
   SPDX-FileCopyrightText: 2020-2023 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "createvideomessagewidgettest.h"
#include "dialogs/createvideomessagewidget.h"
#include <QTest>
#include <QToolButton>
#include <QVBoxLayout>
QTEST_MAIN(CreateVideoMessageWidgetTest)

CreateVideoMessageWidgetTest::CreateVideoMessageWidgetTest(QObject *parent)
    : QObject(parent)
{
}

void CreateVideoMessageWidgetTest::shouldHaveDefaultValues()
{
    CreateVideoMessageWidget w;

    auto mainLayout = w.findChild<QVBoxLayout *>(QStringLiteral("mainLayout"));
    QVERIFY(mainLayout);
    QCOMPARE(mainLayout->contentsMargins(), QMargins{});

    auto mStopButton = w.findChild<QToolButton *>(QStringLiteral("mStopButton"));
    QVERIFY(mStopButton);

    auto mPauseButton = w.findChild<QToolButton *>(QStringLiteral("mStopButton"));
    QVERIFY(mPauseButton);

    auto mRecordButton = w.findChild<QToolButton *>(QStringLiteral("mRecordButton"));
    QVERIFY(mRecordButton);
}
