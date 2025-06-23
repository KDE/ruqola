/*
   SPDX-FileCopyrightText: 2020-2025 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "playsoundwidgettest.h"
using namespace Qt::Literals::StringLiterals;

#include "dialogs/playsoundwidget.h"
#include <KMessageWidget>
#include <QLabel>
#include <QPushButton>
#include <QSlider>
#include <QStandardPaths>
#include <QTest>
#include <QToolButton>
#include <QVBoxLayout>
QTEST_MAIN(PlaySoundWidgetTest)

PlaySoundWidgetTest::PlaySoundWidgetTest(QObject *parent)
    : QObject(parent)
{
    QStandardPaths::setTestModeEnabled(true);
}

void PlaySoundWidgetTest::shouldHaveDefaultValues()
{
    PlaySoundWidget w(nullptr);

    auto mainLayout = w.findChild<QVBoxLayout *>(u"mainLayout"_s);
    QVERIFY(mainLayout);
    QCOMPARE(mainLayout->contentsMargins(), QMargins{});

    auto playerLayout = w.findChild<QHBoxLayout *>(u"playerLayout"_s);
    QVERIFY(playerLayout);
    QCOMPARE(playerLayout->contentsMargins(), QMargins{});

    auto mPlayButton = w.findChild<QPushButton *>(u"mPlayButton"_s);
    QVERIFY(mPlayButton);
    QVERIFY(!mPlayButton->isEnabled());

    auto mPositionSlider = w.findChild<QSlider *>(u"mPositionSlider"_s);
    QVERIFY(mPositionSlider);

    auto mSoundButton = w.findChild<QToolButton *>(u"mSoundButton"_s);
    QVERIFY(mSoundButton);
    QVERIFY(mSoundButton->isCheckable());
    QVERIFY(!mSoundButton->icon().isNull());

    auto mSoundSlider = w.findChild<QSlider *>(u"mSoundSlider"_s);
    QVERIFY(mSoundSlider);
    QCOMPARE(mSoundSlider->value(), 50);
    QCOMPARE(mSoundSlider->minimum(), 0);
    QCOMPARE(mSoundSlider->maximum(), 100);

    auto mLabelDuration = w.findChild<QLabel *>(u"mLabelDuration"_s);
    QVERIFY(mLabelDuration);
    QVERIFY(mLabelDuration->text().isEmpty());
    QCOMPARE(mLabelDuration->textFormat(), Qt::PlainText);

    auto mMessageWidget = w.findChild<KMessageWidget *>(u"mMessageWidget"_s);
    QVERIFY(mMessageWidget);
    QVERIFY(!mMessageWidget->isCloseButtonVisible());
    QVERIFY(mMessageWidget->wordWrap());
    QCOMPARE(mMessageWidget->messageType(), KMessageWidget::Error);

    auto mLabelPercentSound = w.findChild<QLabel *>(u"mLabelPercentSound"_s);
    QVERIFY(mLabelPercentSound);
    QVERIFY(!mLabelPercentSound->text().isEmpty());
    QCOMPARE(mLabelPercentSound->textFormat(), Qt::PlainText);
}

#include "moc_playsoundwidgettest.cpp"
