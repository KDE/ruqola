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

#include "createnewserverwidgettest.h"
#include "dialogs/createnewserverwidget.h"
#include <KPasswordLineEdit>
#include <QFormLayout>
#include <QLineEdit>
#include <QTest>
QTEST_MAIN(CreateNewServerWidgetTest)
CreateNewServerWidgetTest::CreateNewServerWidgetTest(QObject *parent)
    : QObject(parent)
{
}

void CreateNewServerWidgetTest::shouldHaveDefaultValues()
{
    CreateNewServerWidget w;

    auto mainLayout = w.findChild<QFormLayout *>(QStringLiteral("mainLayout"));
    QVERIFY(mainLayout);
    QCOMPARE(mainLayout->contentsMargins(), {});

    auto mAccountName = w.findChild<QLineEdit *>(QStringLiteral("mAccountName"));
    QVERIFY(mAccountName);
    QVERIFY(mAccountName->text().isEmpty());
    QVERIFY(mAccountName->isClearButtonEnabled());

    auto mServerName = w.findChild<QLineEdit *>(QStringLiteral("mServerName"));
    QVERIFY(mServerName);
    QVERIFY(mServerName->text().isEmpty());
    QVERIFY(mServerName->isClearButtonEnabled());

    auto mUserName = w.findChild<QLineEdit *>(QStringLiteral("mUserName"));
    QVERIFY(mUserName);
    QVERIFY(mUserName->text().isEmpty());
    QVERIFY(mUserName->isClearButtonEnabled());

    QVERIFY(w.accountInfo().userName.isEmpty());
    QVERIFY(w.accountInfo().serverUrl.isEmpty());
    QVERIFY(w.accountInfo().accountName.isEmpty());

    auto mPasswordLineEdit = w.findChild<KPasswordLineEdit *>(QStringLiteral("mPasswordLineEdit"));
    QVERIFY(mPasswordLineEdit);
}
