/*
   SPDX-FileCopyrightText: 2020-2022 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "directchannelinfowidgettest.h"
#include "dialogs/directchannelinfowidget.h"
#include <QFormLayout>
#include <QLabel>
#include <QTest>
QTEST_MAIN(DirectChannelInfoWidgetTest)
DirectChannelInfoWidgetTest::DirectChannelInfoWidgetTest(QObject *parent)
    : QObject(parent)
{
}

void DirectChannelInfoWidgetTest::shouldHaveDefaultValues()
{
    DirectChannelInfoWidget w(nullptr);

    auto mainLayout = w.findChild<QFormLayout *>(QStringLiteral("mainLayout"));
    QVERIFY(mainLayout);
    QCOMPARE(mainLayout->contentsMargins(), QMargins{});

    auto mAvatar = w.findChild<QLabel *>(QStringLiteral("mAvatar"));
    QVERIFY(mAvatar);
    QVERIFY(mAvatar->text().isEmpty());

    auto mName = w.findChild<QLabel *>(QStringLiteral("mName"));
    QVERIFY(mName);
    QVERIFY(mName->text().isEmpty());
    QCOMPARE(mName->textInteractionFlags(), Qt::TextBrowserInteraction);

    auto mUserName = w.findChild<QLabel *>(QStringLiteral("mUserName"));
    QVERIFY(mUserName);
    QVERIFY(mUserName->text().isEmpty());
    QCOMPARE(mUserName->textInteractionFlags(), Qt::TextBrowserInteraction);

    auto mStatus = w.findChild<QLabel *>(QStringLiteral("mStatus"));
    QVERIFY(mStatus);
    QVERIFY(mStatus->text().isEmpty());
    QCOMPARE(mStatus->textInteractionFlags(), Qt::TextBrowserInteraction);

    auto mCustomStatus = w.findChild<QLabel *>(QStringLiteral("mCustomStatus"));
    QVERIFY(mCustomStatus);
    QVERIFY(mCustomStatus->text().isEmpty());
    QCOMPARE(mCustomStatus->textInteractionFlags(), Qt::TextBrowserInteraction);

    auto mTimeZone = w.findChild<QLabel *>(QStringLiteral("mTimeZone"));
    QVERIFY(mTimeZone);
    QVERIFY(mTimeZone->text().isEmpty());
    QCOMPARE(mTimeZone->textInteractionFlags(), Qt::TextBrowserInteraction);

    auto mRoles = w.findChild<QLabel *>(QStringLiteral("mRoles"));
    QVERIFY(mRoles);
    QVERIFY(mRoles->text().isEmpty());
    QCOMPARE(mRoles->textInteractionFlags(), Qt::TextBrowserInteraction);

    auto mCreateAt = w.findChild<QLabel *>(QStringLiteral("mCreateAt"));
    QVERIFY(mCreateAt);
    QVERIFY(mCreateAt->text().isEmpty());
    QCOMPARE(mCreateAt->textInteractionFlags(), Qt::TextBrowserInteraction);

    auto mLastLogin = w.findChild<QLabel *>(QStringLiteral("mLastLogin"));
    QVERIFY(mLastLogin);
    QVERIFY(mLastLogin->text().isEmpty());
    QCOMPARE(mLastLogin->textInteractionFlags(), Qt::TextBrowserInteraction);

    auto mEmailsInfo = w.findChild<QLabel *>(QStringLiteral("mEmailsInfo"));
    QVERIFY(mEmailsInfo);
    QVERIFY(mEmailsInfo->text().isEmpty());
    QCOMPARE(mEmailsInfo->textInteractionFlags(), Qt::TextBrowserInteraction);
    QCOMPARE(mEmailsInfo->textFormat(), Qt::RichText);
    QVERIFY(mEmailsInfo->openExternalLinks());
}
