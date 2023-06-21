/*
   SPDX-FileCopyrightText: 2023 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "exportdataselectaccountpage.h"
#include <QVBoxLayout>

ExportDataSelectAccountPage::ExportDataSelectAccountPage(QWidget *parent)
    : QWizardPage(parent)
{
    auto mainLayout = new QVBoxLayout(this);
    mainLayout->setObjectName(QStringLiteral("mainLayout"));
}

ExportDataSelectAccountPage::~ExportDataSelectAccountPage() = default;

#include "moc_exportdataselectaccountpage.cpp"
