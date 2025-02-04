/*
   SPDX-FileCopyrightText: 2023-2025 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "createvideomessagewizardtest.h"
#include "dialogs/createvideomessagewizard.h"
#include <QStandardPaths>
#include <QTest>
QTEST_MAIN(CreateVideoMessageWizardTest)
CreateVideoMessageWizardTest::CreateVideoMessageWizardTest(QObject *parent)
    : QObject{parent}
{
    QStandardPaths::setTestModeEnabled(true);
}

void CreateVideoMessageWizardTest::shouldHaveDefaultValues()
{
    // CreateVideoMessageWizard w(nullptr);
    // QVERIFY(!w.videoMessageInfo().isValid());
    // TODO
}

#include "moc_createvideomessagewizardtest.cpp"
