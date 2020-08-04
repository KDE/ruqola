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

#include "myaccountprofileconfigurewidgettest.h"
#include "myaccount/myaccountprofileconfigurewidget.h"
#include <QFormLayout>
#include <QLineEdit>
#include <QTest>
QTEST_MAIN(MyAccountProfileConfigureWidgetTest)

MyAccountProfileConfigureWidgetTest::MyAccountProfileConfigureWidgetTest(QObject *parent)
    : QObject(parent)
{
}

void MyAccountProfileConfigureWidgetTest::shouldHaveDefaultValues()
{
    MyAccountProfileConfigureWidget w;

    QFormLayout *mainLayout = w.findChild<QFormLayout *>(QStringLiteral("mainLayout"));
    QVERIFY(mainLayout);

    QLineEdit *mUserName = w.findChild<QLineEdit *>(QStringLiteral("mUserName"));
    QVERIFY(mUserName);

    QLineEdit *mEmail = w.findChild<QLineEdit *>(QStringLiteral("mEmail"));
    QVERIFY(mEmail);

    QLineEdit *mName = w.findChild<QLineEdit *>(QStringLiteral("mName"));
    QVERIFY(mName);

    QLineEdit *mNickName = w.findChild<QLineEdit *>(QStringLiteral("mNickName"));
    QVERIFY(mNickName);

    QLineEdit *mStatusText = w.findChild<QLineEdit *>(QStringLiteral("mStatusText"));
    QVERIFY(mStatusText);
}
