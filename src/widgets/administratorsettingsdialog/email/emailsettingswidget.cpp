/*
   SPDX-FileCopyrightText: 2022 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/
#include "emailsettingswidget.h"
#include <KLocalizedString>
#include <QCheckBox>
#include <QComboBox>
#include <QFormLayout>
#include <QLabel>
#include <QLineEdit>
#include <QSpinBox>

EmailSettingsWidget::EmailSettingsWidget(RocketChatAccount *account, QWidget *parent)
    : SettingsWidgetBase(account, parent)
    , mSmtpProtocol(new QComboBox(this))
    , mSmtpHost(new QLineEdit(this))
    , mSmtpPort(new QSpinBox(this))
    , mIgnoreTls(new QCheckBox(i18n("IgnoreTLS"), this))
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

    mSmtpHost->setObjectName(QStringLiteral("mSmtpHost"));
    addLineEdit(i18n("Host"), mSmtpHost, QStringLiteral("SMTP_Host"));

    mSmtpPort->setObjectName(QStringLiteral("mSmtpPort"));
    mSmtpPort->setMaximum(99999);
    addSpinbox(i18n("Port"), mSmtpPort, QStringLiteral("SMTP_Port"));

    mIgnoreTls->setObjectName(QStringLiteral("mIgnoreTls"));
    mMainLayout->addWidget(mIgnoreTls);
    connectCheckBox(mIgnoreTls, QStringLiteral("SMTP_IgnoreTLS"));
}

EmailSettingsWidget::~EmailSettingsWidget() = default;

void EmailSettingsWidget::initialize(const QMap<QString, QVariant> &mapSettings)
{
    initializeWidget(mSmtpProtocol, mapSettings, QStringLiteral("mSmtpProtocol"));
    initializeWidget(mSmtpHost, mapSettings, QString());
    initializeWidget(mSmtpPort, mapSettings, 0);
    initializeWidget(mIgnoreTls, mapSettings, true);
}
