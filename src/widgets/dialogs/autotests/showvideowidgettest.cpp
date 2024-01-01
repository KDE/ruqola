/*
   SPDX-FileCopyrightText: 2020-2024 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "showvideowidgettest.h"
#include "dialogs/showvideowidget.h"
#include <KMessageWidget>
#include <QLabel>
#include <QPushButton>
#include <QSlider>
#include <QStandardPaths>
#include <QTest>
#include <QToolButton>
#include <QVBoxLayout>
#include <QVideoWidget>
QTEST_MAIN(ShowVideoWidgetTest)
ShowVideoWidgetTest::ShowVideoWidgetTest(QObject *parent)
    : QObject(parent)
{
    QStandardPaths::setTestModeEnabled(true);
}

void ShowVideoWidgetTest::shouldHaveDefaultValues()
{
    ShowVideoWidget w(nullptr);
    auto mainLayout = w.findChild<QVBoxLayout *>(QStringLiteral("mainLayout"));
    QVERIFY(mainLayout);
    QCOMPARE(mainLayout->contentsMargins(), QMargins{});

    auto videoWidget = w.findChild<QVideoWidget *>(QStringLiteral("videoWidget"));
    QVERIFY(videoWidget);

    QBoxLayout *controlLayout = w.findChild<QHBoxLayout *>(QStringLiteral("controlLayout"));
    QVERIFY(controlLayout);
    QCOMPARE(controlLayout->contentsMargins(), QMargins{});

    auto mPlayButton = w.findChild<QPushButton *>(QStringLiteral("mPlayButton"));
    QVERIFY(mPlayButton);
    QVERIFY(!mPlayButton->isEnabled());

    auto mPositionSlider = w.findChild<QSlider *>(QStringLiteral("mPositionSlider"));
    QVERIFY(mPositionSlider);

    auto mMessageWidget = w.findChild<KMessageWidget *>(QStringLiteral("mMessageWidget"));
    QVERIFY(mMessageWidget);
    QVERIFY(!mMessageWidget->isCloseButtonVisible());
    QVERIFY(mMessageWidget->wordWrap());
    QCOMPARE(mMessageWidget->messageType(), KMessageWidget::Information);

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
}

#include "moc_showvideowidgettest.cpp"
