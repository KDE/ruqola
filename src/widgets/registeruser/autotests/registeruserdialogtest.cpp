/*
   SPDX-FileCopyrightText: 2020-2026 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "registeruserdialogtest.h"
using namespace Qt::Literals::StringLiterals;

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

    auto mainLayout = w.findChild<QVBoxLayout *>(u"mainLayout"_s);
    QVERIFY(mainLayout);

    auto mRegisterUserWidget = w.findChild<RegisterUserWidget *>(u"mRegisterUserWidget"_s);
    QVERIFY(mRegisterUserWidget);

    auto button = w.findChild<QDialogButtonBox *>(u"button"_s);
    QVERIFY(button);
}

#include "moc_registeruserdialogtest.cpp"
