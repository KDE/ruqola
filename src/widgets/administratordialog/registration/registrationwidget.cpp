/*
   SPDX-FileCopyrightText: 2023-2024 Laurent Montel <montel.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "registrationwidget.h"

#include <KLocalizedString>
#include <QVBoxLayout>

RegistrationWidget::RegistrationWidget(QWidget *parent)
    : QWidget{parent}
{
    auto mainLayout = new QVBoxLayout(this);
    mainLayout->setObjectName(QStringLiteral("mainLayout"));
    mainLayout->setContentsMargins({});
}

RegistrationWidget::~RegistrationWidget() = default;

#include "moc_registrationwidget.cpp"
