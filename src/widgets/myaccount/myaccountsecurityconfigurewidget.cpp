/*
   SPDX-FileCopyrightText: 2021 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "myaccountsecurityconfigurewidget.h"

#include <QVBoxLayout>

MyAccountSecurityConfigureWidget::MyAccountSecurityConfigureWidget(QWidget *parent)
    : QWidget{parent}
{
    auto topLayout = new QVBoxLayout(this);
    topLayout->setObjectName(QStringLiteral("topLayout"));
    topLayout->setContentsMargins({});
}

MyAccountSecurityConfigureWidget::~MyAccountSecurityConfigureWidget() = default;
