/*
   SPDX-FileCopyrightText: 2023 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/
#include "whatsnewdialogtest.h"
#include "whatsnewdialog/whatsnewdialog.h"
#include <QStandardPaths>
#include <QTest>
QTEST_MAIN(WhatsNewDialogTest)
WhatsNewDialogTest::WhatsNewDialogTest(QObject *parent)
    : QObject{parent}
{
    QStandardPaths::setTestModeEnabled(true);
}

void WhatsNewDialogTest::shouldHaveDefaultValues()
{
    WhatsNewDialog d;
    // TODO
}
