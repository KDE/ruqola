/*
   SPDX-FileCopyrightText: 2022 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "bannermessagewidgettest.h"
#include "bannerinfodialog/bannermessagewidget.h"
#include <QTest>
QTEST_MAIN(BannerMessageWidgetTest)
BannerMessageWidgetTest::BannerMessageWidgetTest(QObject *parent)
    : QObject{parent}
{
}

void BannerMessageWidgetTest::shouldHaveDefaultValues()
{
    BannerMessageWidget w;
    w.show();
    QVERIFY(w.wordWrap());
    QCOMPARE(w.messageType(), KMessageWidget::Information);
    QVERIFY(w.bannerInfos().isEmpty());
}
