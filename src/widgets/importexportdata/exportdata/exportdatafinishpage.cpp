/*
   SPDX-FileCopyrightText: 2023 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "exportdatafinishpage.h"
#include <KLocalizedString>
#include <QLabel>
#include <QVBoxLayout>

ExportDataFinishPage::ExportDataFinishPage(QWidget *parent)
    : QWizardPage(parent)
{
    auto mainLayout = new QVBoxLayout(this);
    mainLayout->setObjectName(QStringLiteral("mainLayout"));
    // TODO add Label
}

ExportDataFinishPage::~ExportDataFinishPage() = default;

#include "moc_exportdatafinishpage.cpp"
