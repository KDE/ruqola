/*
   SPDX-FileCopyrightText: 2022 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "configureautocorrectionwidget.h"
#include "autocorrection/widgets/autocorrectionwidget.h"

#include <QVBoxLayout>

ConfigureAutoCorrectionWidget::ConfigureAutoCorrectionWidget(QWidget *parent)
    : QWidget{parent}
    , mAutoCorrectionWidget(new PimCommonAutoCorrection::AutoCorrectionWidget(this))
{
    auto mainLayout = new QVBoxLayout(this);
    mainLayout->setObjectName(QStringLiteral("mainLayout"));
    mainLayout->setContentsMargins({});
    mainLayout->addWidget(mAutoCorrectionWidget);
}

ConfigureAutoCorrectionWidget::~ConfigureAutoCorrectionWidget() = default;

void ConfigureAutoCorrectionWidget::save()
{
    mAutoCorrectionWidget->writeConfig();
}

void ConfigureAutoCorrectionWidget::load()
{
    mAutoCorrectionWidget->loadConfig();
}
