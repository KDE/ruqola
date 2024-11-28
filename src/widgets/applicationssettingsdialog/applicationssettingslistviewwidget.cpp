/*
   SPDX-FileCopyrightText: 2024 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "applicationssettingslistviewwidget.h"
#include "appscountinfowidget.h"
#include <QVBoxLayout>
using namespace Qt::Literals::StringLiterals;
ApplicationsSettingsListViewWidget::ApplicationsSettingsListViewWidget(QWidget *parent)
    : QWidget{parent}
    , mAppsCountInfoWidget(new AppsCountInfoWidget(AppsCountInfoWidget::InfoType::Unknown, this)) // TODO
{
    auto mainLayout = new QVBoxLayout(this);
    mainLayout->setObjectName("mainLayout"_L1);
    mainLayout->setContentsMargins({});

    mAppsCountInfoWidget->setObjectName("mAppsCountInfoWidget"_L1);
    mainLayout->addWidget(mAppsCountInfoWidget);
}

ApplicationsSettingsListViewWidget::~ApplicationsSettingsListViewWidget() = default;

#include "moc_applicationssettingslistviewwidget.cpp"
