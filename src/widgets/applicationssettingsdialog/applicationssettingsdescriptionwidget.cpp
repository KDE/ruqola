/*
   SPDX-FileCopyrightText: 2024-2026 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "applicationssettingsdescriptionwidget.h"
#include <QTextBrowser>
#include <QVBoxLayout>
using namespace Qt::Literals::StringLiterals;
ApplicationsSettingsDescriptionWidget::ApplicationsSettingsDescriptionWidget(QWidget *parent)
    : QWidget{parent}
    , mTextBrowser(new QTextBrowser(this))
{
    auto mainLayout = new QVBoxLayout(this);
    mainLayout->setObjectName("mainLayout"_L1);
    mainLayout->setContentsMargins({});

    mTextBrowser->setObjectName("mTextBrowser"_L1);
    mainLayout->addWidget(mTextBrowser);
    mTextBrowser->setOpenExternalLinks(true);
}

ApplicationsSettingsDescriptionWidget::~ApplicationsSettingsDescriptionWidget() = default;

void ApplicationsSettingsDescriptionWidget::setDescription(const QString &desc)
{
    mTextBrowser->setText(desc);
}

#include "moc_applicationssettingsdescriptionwidget.cpp"
