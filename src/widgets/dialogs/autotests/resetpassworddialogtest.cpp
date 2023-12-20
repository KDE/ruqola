/*
   SPDX-FileCopyrightText: 2023 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "resetpassworddialogtest.h"
#include "dialogs/resetpassworddialog.h"
#include <QTest>
QTEST_MAIN(ResetPasswordDialogTest)
ResetPasswordDialogTest::ResetPasswordDialogTest(QObject *parent)
    : QObject{parent}
{
}

void ResetPasswordDialogTest::shouldHaveDefaultValues()
{
    ResetPasswordDialog d;
    // TODO
}
