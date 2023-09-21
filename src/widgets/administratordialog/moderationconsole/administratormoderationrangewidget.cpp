/*
   SPDX-FileCopyrightText: 2023 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/
#include "administratormoderationrangewidget.h"
#include <KLocalizedString>
#include <QHBoxLayout>
#include <QLabel>

AdministratorModerationRangeWidget::AdministratorModerationRangeWidget(QWidget *parent)
    : QWidget{parent}
{
    auto mainLayout = new QHBoxLayout(this);
    mainLayout->setObjectName(QStringLiteral("mainLayout"));

    auto fromLabel = new QLabel(i18n("From:"), this);
    fromLabel->setObjectName(QStringLiteral("fromLabel"));

    auto toLabel = new QLabel(i18n("To:"), this);
    fromLabel->setObjectName(QStringLiteral("toLabel"));

    mainLayout->addWidget(fromLabel);
    mainLayout->addWidget(toLabel);
}

AdministratorModerationRangeWidget::~AdministratorModerationRangeWidget() = default;

#include "moc_administratormoderationrangewidget.cpp"
