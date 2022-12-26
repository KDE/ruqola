/*
   SPDX-FileCopyrightText: 2021-2022 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "administratoradduserdialogtest.h"
#include "administratordialog/users/administratoradduserdialog.h"
#include "administratordialog/users/administratoradduserwidget.h"
#include <QDialogButtonBox>
#include <QPushButton>
#include <QTest>
#include <QVBoxLayout>
QTEST_MAIN(AdministratorAddUserDialogTest)
AdministratorAddUserDialogTest::AdministratorAddUserDialogTest(QObject *parent)
    : QObject(parent)
{
}

void AdministratorAddUserDialogTest::shouldHaveDefaultValues()
{
    AdministratorAddUserDialog d(nullptr);
    QVERIFY(!d.windowTitle().isEmpty());
    auto mainLayout = d.findChild<QVBoxLayout *>(QStringLiteral("mainLayout"));
    QVERIFY(mainLayout);
    auto mAdministratorWidget = d.findChild<AdministratorAddUserWidget *>(QStringLiteral("mAdministratorWidget"));
    QVERIFY(mAdministratorWidget);

    auto button = d.findChild<QDialogButtonBox *>(QStringLiteral("button"));
    QVERIFY(button);

    QPushButton *buttonOk = button->button(QDialogButtonBox::Ok);
    QVERIFY(buttonOk);
    QVERIFY(!buttonOk->isEnabled());
}
