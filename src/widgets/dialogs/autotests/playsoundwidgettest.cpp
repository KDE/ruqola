/*
   SPDX-FileCopyrightText: 2020 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "playsoundwidgettest.h"
#include "dialogs/playsoundwidget.h"
#include <QLabel>
#include <QPushButton>
#include <QSlider>
#include <QTest>
#include <QToolButton>
#include <QVBoxLayout>
QTEST_MAIN(PlaySoundWidgetTest)

PlaySoundWidgetTest::PlaySoundWidgetTest(QObject *parent)
    : QObject(parent)
{
}

void PlaySoundWidgetTest::shouldHaveDefaultValues()
{
    PlaySoundWidget w;
    auto mainLayout = w.findChild<QHBoxLayout *>(QStringLiteral("mainLayout"));
    QVERIFY(mainLayout);
    QCOMPARE(mainLayout->contentsMargins(), QMargins{});

    auto mPlayButton = w.findChild<QPushButton *>(QStringLiteral("mPlayButton"));
    QVERIFY(mPlayButton);
    QVERIFY(!mPlayButton->isEnabled());

    auto mPositionSlider = w.findChild<QSlider *>(QStringLiteral("mPositionSlider"));
    QVERIFY(mPositionSlider);

    auto mSoundButton = w.findChild<QToolButton *>(QStringLiteral("mSoundButton"));
    QVERIFY(mSoundButton);
    QVERIFY(mSoundButton->isCheckable());
    QVERIFY(!mSoundButton->icon().isNull());

    auto mSoundSlider = w.findChild<QSlider *>(QStringLiteral("mSoundSlider"));
    QVERIFY(mSoundSlider);
    QCOMPARE(mSoundSlider->value(), 100);
    QCOMPARE(mSoundSlider->minimum(), 0);
    QCOMPARE(mSoundSlider->maximum(), 100);

    auto mLabelDuration = w.findChild<QLabel *>(QStringLiteral("mLabelDuration"));
    QVERIFY(mLabelDuration);
    QVERIFY(mLabelDuration->text().isEmpty());
}
