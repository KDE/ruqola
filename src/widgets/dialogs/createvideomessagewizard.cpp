/*
   SPDX-FileCopyrightText: 2020-2023 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "createvideomessagewizard.h"
#include <KLocalizedString>

CreateVideoMessageWizard::CreateVideoMessageWizard(QWidget *parent)
    : QWizard(parent)
{
    setWindowTitle(i18nc("@title:window", "Create Video Message"));
}

CreateVideoMessageWizard::~CreateVideoMessageWizard()
{
}
