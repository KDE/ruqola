/*
   SPDX-FileCopyrightText: 2021-2024 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/
#include "channelinfoprunewidgettest.h"
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
    auto mainLayout = w.findChild<QVBoxLayout *>(QStringLiteral("mainLayout"));
    QVERIFY(mainLayout);
    QCOMPARE(mainLayout->contentsMargins(), QMargins{});

    auto mAutomaticPruneOldMessages = w.findChild<QCheckBox *>(QStringLiteral("mAutomaticPruneOldMessages"));
    QVERIFY(mAutomaticPruneOldMessages);
    QVERIFY(!mAutomaticPruneOldMessages->text().isEmpty());

    auto mOverrideGlobalRetentionPolicy = w.findChild<QCheckBox *>(QStringLiteral("mOverrideGlobalRetentionPolicy"));
    QVERIFY(mOverrideGlobalRetentionPolicy);
    QVERIFY(!mOverrideGlobalRetentionPolicy->text().isEmpty());

    auto mExcludePinnedMessages = w.findChild<QCheckBox *>(QStringLiteral("mExcludePinnedMessages"));
    QVERIFY(mExcludePinnedMessages);
    QVERIFY(!mExcludePinnedMessages->text().isEmpty());

    auto mPruneFileOnlyKeepMessages = w.findChild<QCheckBox *>(QStringLiteral("mPruneFileOnlyKeepMessages"));
    QVERIFY(mPruneFileOnlyKeepMessages);
    QVERIFY(!mPruneFileOnlyKeepMessages->text().isEmpty());

    auto label = w.findChild<QLabel *>(QStringLiteral("label"));
    QVERIFY(label);

    auto mMaximumAgeInDay = w.findChild<QSpinBox *>(QStringLiteral("mMaximumAgeInDay"));
    QVERIFY(mMaximumAgeInDay);
    QCOMPARE(mMaximumAgeInDay->value(), 30);
    QCOMPARE(mMaximumAgeInDay->minimum(), 1);
    QCOMPARE(mMaximumAgeInDay->maximum(), 99999);
}

#include "moc_channelinfoprunewidgettest.cpp"
