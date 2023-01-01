/*
   SPDX-FileCopyrightText: 2020-2023 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "registeruserdialogtest.h"
#include "registeruser/registeruserdialog.h"
#include "registeruser/registeruserwidget.h"

#include <QDialogButtonBox>
#include <QStandardPaths>
#include <QTest>
#include <QVBoxLayout>

QTEST_MAIN(RegisterUserDialogTest)

RegisterUserDialogTest::RegisterUserDialogTest(QObject *parent)
    : QObject(parent)
{
    QStandardPaths::setTestModeEnabled(true);
}

void RegisterUserDialogTest::shouldHaveDefaultValues()
{
    RegisterUserDialog w(nullptr);
    QVERIFY(!w.windowTitle().isEmpty());

    auto mainLayout = w.findChild<QVBoxLayout *>(QStringLiteral("mainLayout"));
    QVERIFY(mainLayout);

    auto mRegisterUserWidget = w.findChild<RegisterUserWidget *>(QStringLiteral("mRegisterUserWidget"));
    QVERIFY(mRegisterUserWidget);

    auto button = w.findChild<QDialogButtonBox *>(QStringLiteral("button"));
    QVERIFY(button);
}
