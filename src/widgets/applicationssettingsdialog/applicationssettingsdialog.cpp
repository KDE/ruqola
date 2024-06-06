/*
   SPDX-FileCopyrightText: 2024 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "applicationssettingsdialog.h"
#include <KLocalizedString>
#include <QVBoxLayout>

ApplicationsSettingsDialog::ApplicationsSettingsDialog(QWidget *parent)
    : QDialog(parent)
{
    setWindowTitle(i18nc("@title:window", "Applications"));
    auto mainLayout = new QVBoxLayout(this);
    mainLayout->setObjectName(QStringLiteral("mainLayout"));
}

ApplicationsSettingsDialog::~ApplicationsSettingsDialog()
{
}
