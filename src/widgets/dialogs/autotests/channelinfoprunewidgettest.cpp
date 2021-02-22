/*
   Copyright (c) 2021 Laurent Montel <montel@kde.org>

   This library is free software; you can redistribute it and/or modify
   it under the terms of the GNU Library General Public License as published
   by the Free Software Foundation; either version 2 of the License or
   ( at your option ) version 3 or, at the discretion of KDE e.V.
   ( which shall act as a proxy as in section 14 of the GPLv3 ), any later version.

   This library is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
   Library General Public License for more details.

   You should have received a copy of the GNU Library General Public License
   along with this library; see the file COPYING.LIB.  If not, write to
   the Free Software Foundation, Inc., 51 Franklin Street, Fifth Floor,
   Boston, MA 02110-1301, USA.
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
    QCOMPARE(mainLayout->contentsMargins(), {});

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
}
