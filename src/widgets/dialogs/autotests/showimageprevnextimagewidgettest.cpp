/*
   SPDX-FileCopyrightText: 2025-2026 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "showimageprevnextimagewidgettest.h"
using namespace Qt::Literals::StringLiterals;

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
    auto mainLayout = d.findChild<QHBoxLayout *>(u"mainLayout"_s);
    QVERIFY(mainLayout);
    QCOMPARE(mainLayout->contentsMargins(), QMargins{});

    auto mNextButton = d.findChild<QToolButton *>(u"mNextButton"_s);
    QVERIFY(mNextButton);
    QVERIFY(!mNextButton->isEnabled());
    QVERIFY(!mNextButton->toolTip().isEmpty());

    auto mPreviousButton = d.findChild<QToolButton *>(u"mPreviousButton"_s);
    QVERIFY(mPreviousButton);
    QVERIFY(!mPreviousButton->isEnabled());
    QVERIFY(!mPreviousButton->toolTip().isEmpty());
}

#include "moc_showimageprevnextimagewidgettest.cpp"
