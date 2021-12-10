/*
   SPDX-FileCopyrightText: 2021 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "myaccount2e2configurewidget.h"
#include <KLocalizedString>
#include <QVBoxLayout>

MyAccount2e2ConfigureWidget::MyAccount2e2ConfigureWidget(QWidget *parent)
    : QWidget{parent}
{
    auto mainLayout = new QVBoxLayout(this);
    mainLayout->setObjectName(QStringLiteral("mainLayout"));
}

MyAccount2e2ConfigureWidget::~MyAccount2e2ConfigureWidget() = default;
