/*
   SPDX-FileCopyrightText: 2025 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "showimageprevnextimagewidgettest.h"
#include "dialogs/showimageprevnextimagewidget.h"
#include <QHBoxLayout>
#include <QTest>
#include <QToolButton>

QTEST_MAIN(ShowImagePrevNextImageWidgetTest)
ShowImagePrevNextImageWidgetTest::ShowImagePrevNextImageWidgetTest(QObject *parent)
    : QObject{parent}
{
}

void ShowImagePrevNextImageWidgetTest::shouldHaveDefaultValues()
{
    ShowImagePrevNextImageWidget d;
    auto mainLayout = d.findChild<QHBoxLayout *>(QStringLiteral("mainLayout"));
    QVERIFY(mainLayout);
    QCOMPARE(mainLayout->contentsMargins(), QMargins{});

    auto mNextButton = d.findChild<QToolButton *>(QStringLiteral("mNextButton"));
    QVERIFY(mNextButton);
    QVERIFY(!mNextButton->isEnabled());
    QVERIFY(!mNextButton->toolTip().isEmpty());

    auto mPreviousButton = d.findChild<QToolButton *>(QStringLiteral("mPreviousButton"));
    QVERIFY(mPreviousButton);
    QVERIFY(!mPreviousButton->isEnabled());
    QVERIFY(!mPreviousButton->toolTip().isEmpty());
}
