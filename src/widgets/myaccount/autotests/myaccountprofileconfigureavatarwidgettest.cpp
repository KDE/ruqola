/*
   SPDX-FileCopyrightText: 2020-2025 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "myaccountprofileconfigureavatarwidgettest.h"
using namespace Qt::Literals::StringLiterals;

#include "myaccount/myaccountprofileconfigureavatarwidget.h"
#include <QHBoxLayout>
#include <QTest>
QTEST_MAIN(MyAccountProfileConfigureAvatarWidgetTest)
MyAccountProfileConfigureAvatarWidgetTest::MyAccountProfileConfigureAvatarWidgetTest(QObject *parent)
    : QObject(parent)
{
}

void MyAccountProfileConfigureAvatarWidgetTest::shouldHaveDefaultValues()
{
    MyAccountProfileConfigureAvatarWidget w(nullptr);

    auto mainLayout = w.findChild<QHBoxLayout *>(u"mainLayout"_s);
    QVERIFY(mainLayout);
    QCOMPARE(mainLayout->contentsMargins(), QMargins{});

    auto mAvatarImage = w.findChild<AvatarImage *>(u"mAvatarImage"_s);
    QVERIFY(mAvatarImage);
}

#include "moc_myaccountprofileconfigureavatarwidgettest.cpp"
