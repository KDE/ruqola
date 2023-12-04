/*
   SPDX-FileCopyrightText: 2020-2023 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "playsoundwidgettest.h"
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

    auto mainLayout = w.findChild<QVBoxLayout *>(QStringLiteral("mainLayout"));
    QVERIFY(mainLayout);
    QCOMPARE(mainLayout->contentsMargins(), QMargins{});

    auto playerLayout = w.findChild<QHBoxLayout *>(QStringLiteral("playerLayout"));
    QVERIFY(playerLayout);
    QCOMPARE(playerLayout->contentsMargins(), QMargins{});

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
    QCOMPARE(mSoundSlider->value(), 50);
    QCOMPARE(mSoundSlider->minimum(), 0);
    QCOMPARE(mSoundSlider->maximum(), 100);

    auto mLabelDuration = w.findChild<QLabel *>(QStringLiteral("mLabelDuration"));
    QVERIFY(mLabelDuration);
    QVERIFY(mLabelDuration->text().isEmpty());
    QCOMPARE(mLabelDuration->textFormat(), Qt::PlainText);

    auto mMessageWidget = w.findChild<KMessageWidget *>(QStringLiteral("mMessageWidget"));
    QVERIFY(mMessageWidget);
    QVERIFY(!mMessageWidget->isCloseButtonVisible());
    QVERIFY(mMessageWidget->wordWrap());
    QCOMPARE(mMessageWidget->messageType(), KMessageWidget::Information);

    auto mLabelPercentSound = w.findChild<QLabel *>(QStringLiteral("mLabelPercentSound"));
    QVERIFY(mLabelPercentSound);
    QVERIFY(!mLabelPercentSound->text().isEmpty());
    QCOMPARE(mLabelPercentSound->textFormat(), Qt::PlainText);
}

#include "moc_playsoundwidgettest.cpp"
