/*
   SPDX-FileCopyrightText: 2020-2024 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
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

#include "moc_myaccountprofileconfigureavatarwidgettest.cpp"
