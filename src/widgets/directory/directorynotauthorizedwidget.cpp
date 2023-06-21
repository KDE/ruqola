/*
   SPDX-FileCopyrightText: 2023 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "directorynotauthorizedwidget.h"
#include <KLocalizedString>
#include <QHBoxLayout>
#include <QLabel>

DirectoryNotAuthorizedWidget::DirectoryNotAuthorizedWidget(QWidget *parent)
    : QWidget{parent}
{
    auto mainLayout = new QHBoxLayout(this);
    mainLayout->setContentsMargins({});
    mainLayout->setObjectName(QStringLiteral("mainLayout"));

    auto label = new QLabel(QStringLiteral("<qt><b>") + i18n("You are not authorized to view this page.") + QStringLiteral("</b></qt>"), this);
    label->setObjectName(QStringLiteral("label"));
    label->setTextFormat(Qt::RichText);
    mainLayout->addWidget(label, 0, Qt::AlignCenter);
}

DirectoryNotAuthorizedWidget::~DirectoryNotAuthorizedWidget() = default;

#include "moc_directorynotauthorizedwidget.cpp"
