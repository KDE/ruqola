/*
   SPDX-FileCopyrightText: 2022 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "retentionpolicysettingswidget.h"

#include <KLocalizedString>

#include <QFormLayout>

RetentionPolicySettingsWidget::RetentionPolicySettingsWidget(RocketChatAccount *account, QWidget *parent)
    : SettingsWidgetBase(account, parent)
{
    auto mainLayout = new QFormLayout(this);
    mainLayout->setObjectName(QStringLiteral("mainLayout"));
    mainLayout->setContentsMargins({});
}

RetentionPolicySettingsWidget::~RetentionPolicySettingsWidget() = default;

void RetentionPolicySettingsWidget::initialize()
{
}
