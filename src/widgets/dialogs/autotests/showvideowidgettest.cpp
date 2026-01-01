/*
   SPDX-FileCopyrightText: 2020-2026 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "showvideowidgettest.h"
using namespace Qt::Literals::StringLiterals;

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
    auto mainLayout = w.findChild<QVBoxLayout *>(u"mainLayout"_s);
    QVERIFY(mainLayout);
    QCOMPARE(mainLayout->contentsMargins(), QMargins{});

    auto videoWidget = w.findChild<QVideoWidget *>(u"videoWidget"_s);
    QVERIFY(videoWidget);

    QBoxLayout *controlLayout = w.findChild<QHBoxLayout *>(u"controlLayout"_s);
    QVERIFY(controlLayout);
    QCOMPARE(controlLayout->contentsMargins(), QMargins{});

    auto mPlayButton = w.findChild<QPushButton *>(u"mPlayButton"_s);
    QVERIFY(mPlayButton);
    QVERIFY(!mPlayButton->isEnabled());

    auto mPositionSlider = w.findChild<QSlider *>(u"mPositionSlider"_s);
    QVERIFY(mPositionSlider);

    auto mMessageWidget = w.findChild<KMessageWidget *>(u"mMessageWidget"_s);
    QVERIFY(mMessageWidget);
    QVERIFY(!mMessageWidget->isCloseButtonVisible());
    QVERIFY(mMessageWidget->wordWrap());
    QCOMPARE(mMessageWidget->messageType(), KMessageWidget::Error);

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
}

#include "moc_showvideowidgettest.cpp"
