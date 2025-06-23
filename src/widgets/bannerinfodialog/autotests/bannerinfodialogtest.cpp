/*
   SPDX-FileCopyrightText: 2022-2025 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "bannerinfodialogtest.h"
using namespace Qt::Literals::StringLiterals;

#include "bannerinfodialog/bannerinfodialog.h"
#include "bannerinfodialog/bannerinfowidget.h"
#include <QDialogButtonBox>
#include <QStandardPaths>
#include <QTest>
#include <QVBoxLayout>

QTEST_MAIN(BannerInfoDialogTest)
BannerInfoDialogTest::BannerInfoDialogTest(QObject *parent)
    : QObject{parent}
{
    QStandardPaths::setTestModeEnabled(true);
}

void BannerInfoDialogTest::shouldHaveDefaultValues()
{
    BannerInfoDialog d(nullptr);
    QVERIFY(!d.windowTitle().isEmpty());
    auto mainLayout = d.findChild<QVBoxLayout *>(u"mainLayout"_s);
    QVERIFY(mainLayout);

    auto mBannerInfoWidget = d.findChild<BannerInfoWidget *>(u"mBannerInfoWidget"_s);
    QVERIFY(mBannerInfoWidget);

    auto button = d.findChild<QDialogButtonBox *>(u"button"_s);
    QVERIFY(button);
}

#include "moc_bannerinfodialogtest.cpp"
