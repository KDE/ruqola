/*
   SPDX-FileCopyrightText: 2020-2021 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: GPL-2.0-or-later
*/

#include "myaccountprofileconfigureavatarwidgettest.h"
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

    auto mainLayout = w.findChild<QHBoxLayout *>(QStringLiteral("mainLayout"));
    QVERIFY(mainLayout);

    auto mAvatarImage = w.findChild<AvatarImage *>(QStringLiteral("mAvatarImage"));
    QVERIFY(mAvatarImage);
}
