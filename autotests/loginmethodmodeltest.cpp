/*
   Copyright (c) 2018-2020 Laurent Montel <montel@kde.org>

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

#include "loginmethodmodeltest.h"
#include "test_model_helpers.h"
#include "model/loginmethodmodel.h"
#include <QSignalSpy>
#include <QTest>

QTEST_MAIN(LoginMethodModelTest)

LoginMethodModelTest::LoginMethodModelTest(QObject *parent)
    : QObject(parent)
{
}

void LoginMethodModelTest::shouldHaveDefaultValue()
{
    LoginMethodModel w;
    QCOMPARE(w.rowCount(), 0);

    QHash<int, QByteArray> roles;
    roles[LoginMethodModel::Name] = QByteArrayLiteral("name");
    roles[LoginMethodModel::IconName] = QByteArrayLiteral("iconname");
    roles[LoginMethodModel::Type] = QByteArrayLiteral("type");
    QCOMPARE(w.roleNames(), roles);
    QCOMPARE(w.currentLoginMethod(), 0);
}

void LoginMethodModelTest::shouldClearModel()
{
    LoginMethodModel w;
    QSignalSpy rowInsertedSpy(&w, &LoginMethodModel::rowsInserted);
    QSignalSpy rowABTInserted(&w, &LoginMethodModel::rowsAboutToBeInserted);
    QSignalSpy rowRemovedSpy(&w, &LoginMethodModel::rowsRemoved);
    QSignalSpy rowABTRemoved(&w, &LoginMethodModel::rowsAboutToBeRemoved);

    QVector<AuthenticationInfo> lst;
    for (int i = 0; i < 8; ++i) {
        AuthenticationInfo info;
        info.setName(QStringLiteral("name%1").arg(i));
        info.setIconName(QStringLiteral("iconname%1").arg(i));
        info.setOauthType(AuthenticationManager::FaceBook);
        lst.append(info);
    }
    w.setAuthenticationInfos(lst);

    QCOMPARE(w.rowCount(), 8);
    QCOMPARE(rowInsertedSpy.count(), 1);
    QCOMPARE(rowABTInserted.count(), 1);
    QCOMPARE(rowRemovedSpy.count(), 0);
    QCOMPARE(rowABTRemoved.count(), 0);
    QCOMPARE(TestModelHelpers::rowSpyToText(rowInsertedSpy), QStringLiteral("0,7"));
    QCOMPARE(TestModelHelpers::rowSpyToText(rowABTInserted), QStringLiteral("0,7"));

    rowInsertedSpy.clear();
    rowABTInserted.clear();
    rowRemovedSpy.clear();
    rowABTRemoved.clear();

    w.clear();
    QCOMPARE(w.rowCount(), 0);
    QCOMPARE(rowInsertedSpy.count(), 0);
    QCOMPARE(rowABTInserted.count(), 0);
    QCOMPARE(rowRemovedSpy.count(), 1);
    QCOMPARE(rowABTRemoved.count(), 1);
    QCOMPARE(TestModelHelpers::rowSpyToText(rowRemovedSpy), QStringLiteral("0,7"));
    QCOMPARE(TestModelHelpers::rowSpyToText(rowABTRemoved), QStringLiteral("0,7"));
}

void LoginMethodModelTest::shouldAssignValues()
{
    LoginMethodModel w;
    QSignalSpy rowInsertedSpy(&w, &LoginMethodModel::rowsInserted);
    QSignalSpy rowABTInserted(&w, &LoginMethodModel::rowsAboutToBeInserted);
    QSignalSpy rowRemovedSpy(&w, &LoginMethodModel::rowsRemoved);
    QSignalSpy rowABTRemoved(&w, &LoginMethodModel::rowsAboutToBeRemoved);

    QVector<AuthenticationInfo> lst;
    for (int i = 0; i < 10; ++i) {
        AuthenticationInfo info;
        info.setName(QStringLiteral("name%1").arg(i));
        info.setIconName(QStringLiteral("iconname%1").arg(i));
        info.setOauthType(AuthenticationManager::FaceBook);
        lst.append(info);
    }
    w.setAuthenticationInfos(lst);
    QCOMPARE(w.rowCount(), 10);
    QCOMPARE(rowInsertedSpy.count(), 1);
    QCOMPARE(rowABTInserted.count(), 1);
    QCOMPARE(rowRemovedSpy.count(), 0);
    QCOMPARE(rowABTRemoved.count(), 0);
    QCOMPARE(TestModelHelpers::rowSpyToText(rowInsertedSpy), QStringLiteral("0,9"));
    QCOMPARE(TestModelHelpers::rowSpyToText(rowABTInserted), QStringLiteral("0,9"));

    //add Empty list
    lst.clear();
    rowInsertedSpy.clear();
    rowABTInserted.clear();
    rowRemovedSpy.clear();
    rowABTRemoved.clear();

    w.setAuthenticationInfos(lst);

    QCOMPARE(w.rowCount(), 0);
    QCOMPARE(rowInsertedSpy.count(), 0);
    QCOMPARE(rowABTInserted.count(), 0);
    QCOMPARE(rowRemovedSpy.count(), 1);
    QCOMPARE(rowABTRemoved.count(), 1);
    QCOMPARE(TestModelHelpers::rowSpyToText(rowRemovedSpy), QStringLiteral("0,9"));
    QCOMPARE(TestModelHelpers::rowSpyToText(rowABTRemoved), QStringLiteral("0,9"));

    //Add same element
    rowInsertedSpy.clear();
    rowABTInserted.clear();
    rowRemovedSpy.clear();
    rowABTRemoved.clear();

    w.setAuthenticationInfos(lst);

    QCOMPARE(w.rowCount(), 0);
    QCOMPARE(rowInsertedSpy.count(), 0);
    QCOMPARE(rowABTInserted.count(), 0);
    QCOMPARE(rowRemovedSpy.count(), 0);
    QCOMPARE(rowABTRemoved.count(), 0);
    QCOMPARE(TestModelHelpers::rowSpyToText(rowRemovedSpy), QString());
    QCOMPARE(TestModelHelpers::rowSpyToText(rowABTRemoved), QString());
    QCOMPARE(TestModelHelpers::rowSpyToText(rowInsertedSpy), QString());
    QCOMPARE(TestModelHelpers::rowSpyToText(rowABTInserted), QString());

    //Test add same number of element.
    lst.clear();
    rowInsertedSpy.clear();
    rowABTInserted.clear();
    rowRemovedSpy.clear();
    rowABTRemoved.clear();

    for (int i = 0; i < 5; ++i) {
        AuthenticationInfo info;
        info.setName(QStringLiteral("name%1").arg(i));
        info.setIconName(QStringLiteral("iconname%1").arg(i));
        info.setOauthType(AuthenticationManager::FaceBook);
        lst.append(info);
    }
    w.setAuthenticationInfos(lst);

    QCOMPARE(w.rowCount(), 5);
    QCOMPARE(rowInsertedSpy.count(), 1);
    QCOMPARE(rowABTInserted.count(), 1);
    QCOMPARE(rowRemovedSpy.count(), 0);
    QCOMPARE(rowABTRemoved.count(), 0);
    QCOMPARE(TestModelHelpers::rowSpyToText(rowInsertedSpy), QStringLiteral("0,4"));
    QCOMPARE(TestModelHelpers::rowSpyToText(rowABTInserted), QStringLiteral("0,4"));

    rowInsertedSpy.clear();
    rowABTInserted.clear();
    rowRemovedSpy.clear();
    rowABTRemoved.clear();
    w.setAuthenticationInfos(lst);

    QCOMPARE(rowInsertedSpy.count(), 1);
    QCOMPARE(rowABTInserted.count(), 1);
    QCOMPARE(rowRemovedSpy.count(), 1);
    QCOMPARE(rowABTRemoved.count(), 1);
    QCOMPARE(TestModelHelpers::rowSpyToText(rowRemovedSpy), QStringLiteral("0,4"));
    QCOMPARE(TestModelHelpers::rowSpyToText(rowABTRemoved), QStringLiteral("0,4"));
    QCOMPARE(TestModelHelpers::rowSpyToText(rowInsertedSpy), QStringLiteral("0,4"));
    QCOMPARE(TestModelHelpers::rowSpyToText(rowABTInserted), QStringLiteral("0,4"));
}

void LoginMethodModelTest::shouldEmitLoginMethodChanged()
{
    LoginMethodModel w;
    QSignalSpy loginMethodChanged(&w, &LoginMethodModel::currentLoginMethodChanged);

    w.setCurrentLoginMethod(3);
    QCOMPARE(loginMethodChanged.count(), 1);

    loginMethodChanged.clear();

    w.setCurrentLoginMethod(3);
    QCOMPARE(loginMethodChanged.count(), 0);

    loginMethodChanged.clear();

    w.setCurrentLoginMethod(1);
    QCOMPARE(loginMethodChanged.count(), 1);
}
