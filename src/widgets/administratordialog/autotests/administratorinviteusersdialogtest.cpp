/*
   SPDX-FileCopyrightText: 2022-2026 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "administratorinviteusersdialogtest.h"
using namespace Qt::Literals::StringLiterals;

#include "administratordialog/users/administratorinviteusersdialog.h"
#include "administratordialog/users/administratorinviteuserswidget.h"

#include <QDialogButtonBox>
#include <QTest>
#include <QVBoxLayout>

QTEST_MAIN(AdministratorInviteUsersDialogTest)
AdministratorInviteUsersDialogTest::AdministratorInviteUsersDialogTest(QObject *parent)
    : QObject{parent}
{
}

void AdministratorInviteUsersDialogTest::shouldHaveDefaultValues()
{
    AdministratorInviteUsersDialog w;
    QVERIFY(w.emails().isEmpty());

    QVERIFY(!w.windowTitle().isEmpty());
    auto mainLayout = w.findChild<QVBoxLayout *>(u"mainLayout"_s);
    QVERIFY(mainLayout);

    auto mCreateWidget = w.findChild<AdministratorInviteUsersWidget *>(u"mInviteUsersWidget"_s);
    QVERIFY(mCreateWidget);

    auto buttonBox = w.findChild<QDialogButtonBox *>(u"button"_s);
    QVERIFY(buttonBox);
}

#include "moc_administratorinviteusersdialogtest.cpp"
