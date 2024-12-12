/*
   SPDX-FileCopyrightText: 2024 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "applicationssettingsdescriptiontabwidget.h"
#include "applicationssettingsdescriptionwidget.h"
#include <KLocalizedString>
#include <QTabWidget>
#include <QVBoxLayout>
using namespace Qt::Literals::StringLiterals;

ApplicationsSettingsDescriptionTabWidget::ApplicationsSettingsDescriptionTabWidget(QWidget *parent)
    : QWidget(parent)
    , mTabWidget(new QTabWidget(this))
    , mDescriptionWidget(new ApplicationsSettingsDescriptionWidget(this))
{
    mTabWidget->setObjectName("mTabWidget"_L1);

    auto mainLayout = new QVBoxLayout(this);
    mainLayout->setObjectName(QStringLiteral("mainLayout"));
    mainLayout->addWidget(mTabWidget);

    mDescriptionWidget->setObjectName("mDescriptionWidget"_L1);
    mTabWidget->addTab(mDescriptionWidget, i18n("Description"));
}

ApplicationsSettingsDescriptionTabWidget::~ApplicationsSettingsDescriptionTabWidget() = default;

void ApplicationsSettingsDescriptionTabWidget::setDescription(const QString &desc)
{
    mDescriptionWidget->setDescription(desc);
}

#include "moc_applicationssettingsdescriptiontabwidget.cpp"
