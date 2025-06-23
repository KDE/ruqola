/*
   SPDX-FileCopyrightText: 2023-2025 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "registrationwidget.h"
using namespace Qt::Literals::StringLiterals;

#include <KLocalizedString>
#include <QVBoxLayout>

RegistrationWidget::RegistrationWidget(QWidget *parent)
    : QWidget{parent}
{
    auto mainLayout = new QVBoxLayout(this);
    mainLayout->setObjectName(u"mainLayout"_s);
    mainLayout->setContentsMargins({});
}

RegistrationWidget::~RegistrationWidget() = default;

#include "moc_registrationwidget.cpp"
