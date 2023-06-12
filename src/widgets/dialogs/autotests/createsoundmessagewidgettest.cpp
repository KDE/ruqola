/*
   SPDX-FileCopyrightText: 2023 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "createsoundmessagewidgettest.h"
#include "dialogs/createsoundmessagewidget.h"
#include <QComboBox>
#include <QLabel>
#include <QTest>
#include <QToolButton>
#include <QVBoxLayout>

QTEST_MAIN(CreateSoundMessageWidgetTest)
CreateSoundMessageWidgetTest::CreateSoundMessageWidgetTest(QObject *parent)
    : QObject{parent}
{
}

void CreateSoundMessageWidgetTest::shouldHaveDefaultValues()
{
    CreateSoundMessageWidget w;

    auto mainLayout = w.findChild<QVBoxLayout *>(QStringLiteral("mainLayout"));
    QVERIFY(mainLayout);
    QCOMPARE(mainLayout->contentsMargins(), QMargins{});

    auto mDeviceComboBox = w.findChild<QComboBox *>(QStringLiteral("mDeviceComboBox"));
    QVERIFY(mDeviceComboBox);

    auto mRecordButton = w.findChild<QToolButton *>(QStringLiteral("mRecordButton"));
    QVERIFY(mRecordButton);

    auto mPauseButton = w.findChild<QToolButton *>(QStringLiteral("mPauseButton"));
    QVERIFY(mPauseButton);

    auto mStopButton = w.findChild<QToolButton *>(QStringLiteral("mStopButton"));
    QVERIFY(mStopButton);

    auto mLabelDuration = w.findChild<QLabel *>(QStringLiteral("mLabelDuration"));
    QVERIFY(mLabelDuration);
    QCOMPARE(mLabelDuration->textFormat(), Qt::PlainText);
}
