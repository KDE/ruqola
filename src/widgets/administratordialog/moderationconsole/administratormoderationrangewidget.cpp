/*
   SPDX-FileCopyrightText: 2023 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/
#include "administratormoderationrangewidget.h"
#include <KLocalizedString>
#include <QDateTimeEdit>
#include <QHBoxLayout>
#include <QLabel>

AdministratorModerationRangeWidget::AdministratorModerationRangeWidget(QWidget *parent)
    : QWidget{parent}
    , mFromDateTime(new QDateTimeEdit(this))
    , mToDateTime(new QDateTimeEdit(this))
{
    auto mainLayout = new QHBoxLayout(this);
    mainLayout->setObjectName(QStringLiteral("mainLayout"));

    mFromDateTime->setObjectName(QStringLiteral("mFromDateTime"));
    mToDateTime->setObjectName(QStringLiteral("mToDateTime"));

    auto fromLabel = new QLabel(i18n("From:"), this);
    fromLabel->setObjectName(QStringLiteral("fromLabel"));

    auto toLabel = new QLabel(i18n("To:"), this);
    fromLabel->setObjectName(QStringLiteral("toLabel"));

    mainLayout->addWidget(fromLabel);
    mainLayout->addWidget(mFromDateTime);
    mainLayout->addWidget(toLabel);
    mainLayout->addWidget(mToDateTime);
}

AdministratorModerationRangeWidget::~AdministratorModerationRangeWidget() = default;

#include "moc_administratormoderationrangewidget.cpp"
