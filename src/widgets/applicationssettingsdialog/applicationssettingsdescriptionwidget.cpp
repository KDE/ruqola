/*
   SPDX-FileCopyrightText: 2024 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "applicationssettingsdescriptionwidget.h"
#include <QVBoxLayout>

ApplicationsSettingsDescriptionWidget::ApplicationsSettingsDescriptionWidget(QWidget *parent)
    : QWidget{parent}
{
    auto mainLayout = new QVBoxLayout(this);
    mainLayout->setObjectName(QStringLiteral("mainLayout"));
    mainLayout->setContentsMargins({});
}

ApplicationsSettingsDescriptionWidget::~ApplicationsSettingsDescriptionWidget() = default;

#include "moc_applicationssettingsdescriptionwidget.cpp"
