/*
   SPDX-FileCopyrightText: 2023 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "removeaccountdialogtest.h"
#include "configuredialog/removeaccountdialog.h"
#include <QTest>
QTEST_MAIN(RemoveAccountDialogTest)
RemoveAccountDialogTest::RemoveAccountDialogTest(QObject *parent)
    : QObject{parent}
{
}

void RemoveAccountDialogTest::shouldHaveDefaultValues()
{
    RemoveAccountDialog d;
    // TODO
}
