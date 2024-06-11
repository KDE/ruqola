/*
   SPDX-FileCopyrightText: 2024 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "applicationssettingssearchwidget.h"
#include <QHBoxLayout>

ApplicationsSettingsSearchWidget::ApplicationsSettingsSearchWidget(QWidget *parent)
    : QWidget{parent}
{
    auto mainLayout = new QHBoxLayout(this);
    mainLayout->setContentsMargins({});
    mainLayout->setObjectName({});
    // TODO
}

ApplicationsSettingsSearchWidget::~ApplicationsSettingsSearchWidget() = default;
