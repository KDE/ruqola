/*
   Copyright (c) 2021 Laurent Montel <montel@kde.org>

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

#include "administratoradduserwidgettest.h"
#include "administratordialog/users/administratoradduserwidget.h"
#include "administratordialog/users/rolescombobox.h"
#include <KPasswordLineEdit>
#include <QCheckBox>
#include <QFormLayout>
#include <QLineEdit>
#include <QTest>
QTEST_MAIN(AdministratorAddUserWidgetTest)
AdministratorAddUserWidgetTest::AdministratorAddUserWidgetTest(QObject *parent)
    : QObject(parent)
{
}

void AdministratorAddUserWidgetTest::shouldHaveDefaultValues()
{
    AdministratorAddUserWidget w;
    auto formLayout = w.findChild<QFormLayout *>(QStringLiteral("formLayout"));
    QVERIFY(formLayout);
    auto mName = w.findChild<QLineEdit *>(QStringLiteral("mName"));
    QVERIFY(mName);
    QVERIFY(mName->text().isEmpty());

    auto mUserName = w.findChild<QLineEdit *>(QStringLiteral("mUserName"));
    QVERIFY(mUserName);
    QVERIFY(mUserName->text().isEmpty());

    auto mEmail = w.findChild<QLineEdit *>(QStringLiteral("mEmail"));
    QVERIFY(mEmail);
    QVERIFY(mEmail->text().isEmpty());

    auto mJoinDefaultChannels = w.findChild<QCheckBox *>(QStringLiteral("mJoinDefaultChannels"));
    QVERIFY(mJoinDefaultChannels);
    QVERIFY(!mJoinDefaultChannels->text().isEmpty());
    QVERIFY(!mJoinDefaultChannels->isChecked());

    auto mSendWelcomeEmails = w.findChild<QCheckBox *>(QStringLiteral("mSendWelcomeEmails"));
    QVERIFY(mSendWelcomeEmails);
    QVERIFY(!mSendWelcomeEmails->text().isEmpty());
    QVERIFY(!mSendWelcomeEmails->isChecked());

    auto mPasswordLineEdit = w.findChild<KPasswordLineEdit *>(QStringLiteral("mPasswordLineEdit"));
    QVERIFY(mPasswordLineEdit);
    QVERIFY(mPasswordLineEdit->password().isEmpty());

    auto mRolesComboBox = w.findChild<KPasswordLineEdit *>(QStringLiteral("mRolesComboBox"));
    QVERIFY(mRolesComboBox);
}
