/*
   Copyright (c) 2020-2021 Laurent Montel <montel@kde.org>

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

#include "registeruserwidgettest.h"
#include "registeruser/registeruserwidget.h"
#include <KPasswordLineEdit>
#include <QFormLayout>
#include <QLineEdit>
#include <QPushButton>
#include <QTest>
QTEST_MAIN(RegisterUserWidgetTest)
RegisterUserWidgetTest::RegisterUserWidgetTest(QObject *parent)
    : QObject(parent)
{
}

void RegisterUserWidgetTest::shouldHaveDefaultValues()
{
    RegisterUserWidget w;
    auto mainLayout = w.findChild<QFormLayout *>(QStringLiteral("mainLayout"));
    QCOMPARE(mainLayout->contentsMargins(), {});

    auto mUserName = w.findChild<QLineEdit *>(QStringLiteral("mUserName"));
    QVERIFY(mUserName);
    QVERIFY(mUserName->text().isEmpty());

    auto mEmail = w.findChild<QLineEdit *>(QStringLiteral("mEmail"));
    QVERIFY(mEmail);
    QVERIFY(mEmail->text().isEmpty());

    auto mPasswordLineEdit = w.findChild<KPasswordLineEdit *>(QStringLiteral("mPasswordLineEdit"));
    QVERIFY(mPasswordLineEdit);
    QVERIFY(mPasswordLineEdit->password().isEmpty());

    auto mConfirmPasswordLineEdit = w.findChild<KPasswordLineEdit *>(QStringLiteral("mConfirmPasswordLineEdit"));
    QVERIFY(mConfirmPasswordLineEdit);
    QVERIFY(mConfirmPasswordLineEdit->password().isEmpty());

    auto mRegisterButton = w.findChild<QPushButton *>(QStringLiteral("mRegisterButton"));
    QVERIFY(mRegisterButton);
    QVERIFY(!mRegisterButton->isEnabled());
    QVERIFY(!mRegisterButton->text().isEmpty());
}
