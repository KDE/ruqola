/*
   SPDX-FileCopyrightText: 2020-2023 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "showlistmessagebasedialogtest.h"
#include "dialogs/showlistmessagebasedialog.h"
#include <QStandardPaths>
#include <QTest>
QTEST_MAIN(ShowListMessageBaseDialogTest)

ShowListMessageBaseDialogTest::ShowListMessageBaseDialogTest(QObject *parent)
    : QObject(parent)
{
    QStandardPaths::setTestModeEnabled(true);
}

void ShowListMessageBaseDialogTest::shouldHaveDefaultValues()
{
    // TODO
    ShowListMessageBaseDialog w(nullptr);
}
