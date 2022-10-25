/*
   SPDX-FileCopyrightText: 2022 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/
#include "emailsettingswidget.h"
#include <KLocalizedString>
#include <QFormLayout>
#include <QLabel>

EmailSettingsWidget::EmailSettingsWidget(RocketChatAccount *account, QWidget *parent)
    : SettingsWidgetBase(account, parent)
{
    auto smtpLabel = createBoldLabel(i18n("STMP"));
    smtpLabel->setObjectName(QStringLiteral("smtpLabel"));
    mMainLayout->addWidget(smtpLabel);
}

EmailSettingsWidget::~EmailSettingsWidget() = default;

void EmailSettingsWidget::initialize(const QMap<QString, QVariant> &mapSettings)
{
}
