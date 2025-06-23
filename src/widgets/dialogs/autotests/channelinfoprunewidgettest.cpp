/*
   SPDX-FileCopyrightText: 2021-2025 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/
#include "channelinfoprunewidgettest.h"
using namespace Qt::Literals::StringLiterals;

#include "dialogs/channelinfoprunewidget.h"
#include <QCheckBox>
#include <QLabel>
#include <QSpinBox>
#include <QTest>
#include <QVBoxLayout>
QTEST_MAIN(ChannelInfoPruneWidgetTest)

ChannelInfoPruneWidgetTest::ChannelInfoPruneWidgetTest(QObject *parent)
    : QObject(parent)
{
}

void ChannelInfoPruneWidgetTest::shouldHaveDefaultValues()
{
    ChannelInfoPruneWidget w;
    auto mainLayout = w.findChild<QVBoxLayout *>(u"mainLayout"_s);
    QVERIFY(mainLayout);
    QCOMPARE(mainLayout->contentsMargins(), QMargins{});

    auto mAutomaticPruneOldMessages = w.findChild<QCheckBox *>(u"mAutomaticPruneOldMessages"_s);
    QVERIFY(mAutomaticPruneOldMessages);
    QVERIFY(!mAutomaticPruneOldMessages->text().isEmpty());

    auto mOverrideGlobalRetentionPolicy = w.findChild<QCheckBox *>(u"mOverrideGlobalRetentionPolicy"_s);
    QVERIFY(mOverrideGlobalRetentionPolicy);
    QVERIFY(!mOverrideGlobalRetentionPolicy->text().isEmpty());

    auto mExcludePinnedMessages = w.findChild<QCheckBox *>(u"mExcludePinnedMessages"_s);
    QVERIFY(mExcludePinnedMessages);
    QVERIFY(!mExcludePinnedMessages->text().isEmpty());

    auto mPruneFileOnlyKeepMessages = w.findChild<QCheckBox *>(u"mPruneFileOnlyKeepMessages"_s);
    QVERIFY(mPruneFileOnlyKeepMessages);
    QVERIFY(!mPruneFileOnlyKeepMessages->text().isEmpty());

    auto label = w.findChild<QLabel *>(u"label"_s);
    QVERIFY(label);

    auto mMaximumAgeInDay = w.findChild<QSpinBox *>(u"mMaximumAgeInDay"_s);
    QVERIFY(mMaximumAgeInDay);
    QCOMPARE(mMaximumAgeInDay->value(), 30);
    QCOMPARE(mMaximumAgeInDay->minimum(), 1);
    QCOMPARE(mMaximumAgeInDay->maximum(), 99999);
}

#include "moc_channelinfoprunewidgettest.cpp"
