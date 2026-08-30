/*
   SPDX-FileCopyrightText: 2024-2026 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "applicationssettingsinprogresswidget.h"
#include <KLocalizedString>
#include <QHBoxLayout>
#include <QLabel>
#include <QProgressBar>
using namespace Qt::Literals::StringLiterals;
ApplicationsSettingsInProgressWidget::ApplicationsSettingsInProgressWidget(QWidget *parent)
    : QWidget{parent}
    , mLabel(new QLabel(i18nc("@label:textbox", "Loading in progress…"), this))
    , mProgressBar(new QProgressBar(this))
{
    auto mainLayout = new QHBoxLayout(this);
    mainLayout->setObjectName(u"mainLayout"_s);
    mainLayout->setContentsMargins({});

    mLabel->setObjectName(u"label"_s);
    mainLayout->addWidget(mLabel);

    mProgressBar->setObjectName(u"mProgressBar"_s);
    mainLayout->addWidget(mProgressBar);
    mProgressBar->setValue(0);
    mProgressBar->setMaximum(0);
    mProgressBar->setMinimum(0);
}

ApplicationsSettingsInProgressWidget::~ApplicationsSettingsInProgressWidget() = default;

#include "moc_applicationssettingsinprogresswidget.cpp"
