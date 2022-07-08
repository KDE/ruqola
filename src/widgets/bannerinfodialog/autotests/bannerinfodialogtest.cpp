/*
   SPDX-FileCopyrightText: 2022 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "bannerinfodialogtest.h"
#include "bannerinfodialog/bannerinfodialog.h"
#include "bannerinfodialog/bannerinfowidget.h"
#include <QDialogButtonBox>
#include <QTest>
#include <QVBoxLayout>

QTEST_MAIN(BannerInfoDialogTest)
BannerInfoDialogTest::BannerInfoDialogTest(QObject *parent)
    : QObject{parent}
{
}

void BannerInfoDialogTest::shouldHaveDefaultValues()
{
    BannerInfoDialog d(nullptr);
    QVERIFY(!d.windowTitle().isEmpty());
    auto mainLayout = d.findChild<QVBoxLayout *>(QStringLiteral("mainLayout"));
    QVERIFY(mainLayout);

    auto mBannerInfoWidget = d.findChild<BannerInfoWidget *>(QStringLiteral("mBannerInfoWidget"));
    QVERIFY(mBannerInfoWidget);

    auto button = d.findChild<QDialogButtonBox *>(QStringLiteral("button"));
    QVERIFY(button);
}
