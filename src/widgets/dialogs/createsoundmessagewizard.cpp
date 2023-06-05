/*
   SPDX-FileCopyrightText: 2023 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "createsoundmessagewizard.h"

CreateSoundMessageWizard::CreateSoundMessageWizard(QWidget *parent)
    : QWizard(parent)
{
}

CreateSoundMessageWizard::~CreateSoundMessageWizard() = default;

CreateSoundMessagePage::CreateSoundMessagePage(QWidget *parent)
    : QWizardPage(parent)
{
}

CreateSoundMessagePage::~CreateSoundMessagePage()
{
}

GenerateSoundMessagePage::GenerateSoundMessagePage(QWidget *parent)
    : QWizardPage(parent)
{
}

GenerateSoundMessagePage::~GenerateSoundMessagePage()
{
}
