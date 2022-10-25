/*
   SPDX-FileCopyrightText: 2022 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/
#include "emailsettingswidget.h"
#include <KLocalizedString>
#include <QComboBox>
#include <QFormLayout>
#include <QLabel>
#include <QSpinBox>

EmailSettingsWidget::EmailSettingsWidget(RocketChatAccount *account, QWidget *parent)
    : SettingsWidgetBase(account, parent)
    , mSmtpProtocol(new QComboBox(this))
{
    auto smtpLabel = createBoldLabel(i18n("STMP"));
    smtpLabel->setObjectName(QStringLiteral("smtpLabel"));
    mMainLayout->addWidget(smtpLabel);

    mSmtpProtocol->setObjectName(QStringLiteral("mSmtpProtocol"));
    const QMap<QString, QString> maps = {
        {QStringLiteral("smtp"), i18n("smtp")},
        {QStringLiteral("smtps"), i18n("smtps")},
    };
    addComboBox(i18n("Protocol"), maps, mSmtpProtocol, QStringLiteral("SMTP_Protocol"));
}

EmailSettingsWidget::~EmailSettingsWidget() = default;

void EmailSettingsWidget::initialize(const QMap<QString, QVariant> &mapSettings)
{
    initializeWidget(mSmtpProtocol, mapSettings, QStringLiteral("mSmtpProtocol"));
}
