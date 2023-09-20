/*
   SPDX-FileCopyrightText: 2023 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/
#include "administratormoderationrangewidget.h"
#include <QHBoxLayout>

AdministratorModerationRangeWidget::AdministratorModerationRangeWidget(QWidget *parent)
    : QWidget{parent}
{
    auto mainLayout = new QHBoxLayout(this);
    mainLayout->setObjectName(QStringLiteral("mainLayout"));
    mainLayout->setContentsMargins({});
}

AdministratorModerationRangeWidget::~AdministratorModerationRangeWidget() = default;

#include "moc_administratormoderationrangewidget.cpp"
