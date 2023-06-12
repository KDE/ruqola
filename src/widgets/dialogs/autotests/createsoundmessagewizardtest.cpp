/*
   SPDX-FileCopyrightText: 2023 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "createsoundmessagewizardtest.h"
#include "dialogs/createsoundmessagewizard.h"
#include <QTest>
QTEST_MAIN(CreateSoundMessageWizardTest)
CreateSoundMessageWizardTest::CreateSoundMessageWizardTest(QObject *parent)
    : QObject{parent}
{
}

void CreateSoundMessageWizardTest::shouldHaveDefaultValues()
{
    CreateSoundMessageWizard w;
    // TODO
}
