/*
   SPDX-FileCopyrightText: 2020-2021 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "showvideowidgettest.h"
#include "dialogs/showvideowidget.h"
#include <QLabel>
#include <QPushButton>
#include <QSlider>
#include <QTest>
#include <QToolButton>
#include <QVBoxLayout>
#include <QVideoWidget>
QTEST_MAIN(ShowVideoWidgetTest)
ShowVideoWidgetTest::ShowVideoWidgetTest(QObject *parent)
    : QObject(parent)
{
}

void ShowVideoWidgetTest::shouldHaveDefaultValues()
{
    ShowVideoWidget w;
    auto mainLayout = w.findChild<QVBoxLayout *>(QStringLiteral("mainLayout"));
    QVERIFY(mainLayout);
    QCOMPARE(mainLayout->contentsMargins(), {});

    auto videoWidget = w.findChild<QVideoWidget *>(QStringLiteral("videoWidget"));
    QVERIFY(videoWidget);

    QBoxLayout *controlLayout = w.findChild<QHBoxLayout *>(QStringLiteral("controlLayout"));
    QVERIFY(controlLayout);
    QCOMPARE(controlLayout->contentsMargins(), {});

    auto mPlayButton = w.findChild<QPushButton *>(QStringLiteral("mPlayButton"));
    QVERIFY(mPlayButton);
    QVERIFY(!mPlayButton->isEnabled());

    auto mPositionSlider = w.findChild<QSlider *>(QStringLiteral("mPositionSlider"));
    QVERIFY(mPositionSlider);

    auto mErrorLabel = w.findChild<QLabel *>(QStringLiteral("mErrorLabel"));
    QVERIFY(mErrorLabel);

    auto mSoundButton = w.findChild<QToolButton *>(QStringLiteral("mSoundButton"));
    QVERIFY(mSoundButton);
    QVERIFY(mSoundButton->isCheckable());
    QVERIFY(!mSoundButton->icon().isNull());

    auto mSoundSlider = w.findChild<QSlider *>(QStringLiteral("mSoundSlider"));
    QVERIFY(mSoundSlider);
    QCOMPARE(mSoundSlider->value(), 100);
}
