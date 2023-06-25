/*
   SPDX-FileCopyrightText: 2023 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/
#include "importdataselectaccountpage.h"
#include <KLocalizedString>
#include <QVBoxLayout>

ImportDataSelectAccountPage::ImportDataSelectAccountPage(QWidget *parent)
    : QWizardPage(parent)
{
    auto mainLayout = new QVBoxLayout(this);
    mainLayout->setObjectName(QStringLiteral("mainLayout"));
}

ImportDataSelectAccountPage::~ImportDataSelectAccountPage() = default;

#include "moc_importdataselectaccountpage.cpp"
