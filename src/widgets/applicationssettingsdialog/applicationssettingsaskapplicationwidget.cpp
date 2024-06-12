/*
   SPDX-FileCopyrightText: 2024 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "applicationssettingsaskapplicationwidget.h"
#include <QVBoxLayout>
using namespace Qt::Literals::StringLiterals;
ApplicationsSettingsAskApplicationWidget::ApplicationsSettingsAskApplicationWidget(QWidget *parent)
    : QWidget{parent}
{
    auto mainLayout = new QVBoxLayout(this);
    mainLayout->setObjectName("mainLayout"_L1);
    mainLayout->setContentsMargins({});
}

ApplicationsSettingsAskApplicationWidget::~ApplicationsSettingsAskApplicationWidget() = default;

#include "moc_applicationssettingsaskapplicationwidget.cpp"
