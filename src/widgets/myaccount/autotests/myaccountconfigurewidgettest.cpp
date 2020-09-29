/*
   Copyright (c) 2020 Laurent Montel <montel@kde.org>

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

#include "myaccountconfigurewidgettest.h"
#include "myaccount/myaccountconfigurewidget.h"
#include "myaccount/myaccount2faconfigurewidget.h"
#include "myaccount/myaccountprofileconfigurewidget.h"
#include "myaccount/myaccountpreferenceconfigurewidget.h"
#include <QTabWidget>
#include <QTest>
#include <QVBoxLayout>

QTEST_MAIN(MyAccountConfigureWidgetTest)
MyAccountConfigureWidgetTest::MyAccountConfigureWidgetTest(QObject *parent)
    : QObject(parent)
{
}

void MyAccountConfigureWidgetTest::shouldHaveDefaultValues()
{
    MyAccountConfigureWidget w;

    QVBoxLayout *mainLayout = w.findChild<QVBoxLayout *>(QStringLiteral("mainLayout"));
    QVERIFY(mainLayout);
    QCOMPARE(mainLayout->contentsMargins(), QMargins(0, 0, 0, 0));

    QTabWidget *tabWidget = w.findChild<QTabWidget *>(QStringLiteral("tabWidget"));
    QVERIFY(tabWidget);

    MyAccountProfileConfigureWidget *mMyAccount2ProfileConfigureWidget = w.findChild<MyAccountProfileConfigureWidget *>(QStringLiteral("mMyAccount2ProfileConfigureWidget"));
    QVERIFY(mMyAccount2ProfileConfigureWidget);

    MyAccount2FaConfigureWidget *mMyAccount2FaConfigureWidget = w.findChild<MyAccount2FaConfigureWidget *>(QStringLiteral("mMyAccount2FaConfigureWidget"));
    QVERIFY(mMyAccount2FaConfigureWidget);

    MyAccountPreferenceConfigureWidget *mMyAccountPreferenceConfigureWidget = w.findChild<MyAccountPreferenceConfigureWidget *>(QStringLiteral("mMyAccount2FaConfigureWidget"));
    QVERIFY(mMyAccountPreferenceConfigureWidget);
}
