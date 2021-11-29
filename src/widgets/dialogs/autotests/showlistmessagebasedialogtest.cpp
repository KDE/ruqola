/*
   SPDX-FileCopyrightText: 2020-2021 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: GPL-2.0-or-later
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
