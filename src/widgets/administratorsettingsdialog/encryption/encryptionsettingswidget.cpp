/*
   SPDX-FileCopyrightText: 2022 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "encryptionsettingswidget.h"

#include <QCheckBox>
#include <QFormLayout>

#include <KLocalizedString>

EncryptionSettingsWidget::EncryptionSettingsWidget(RocketChatAccount *account, QWidget *parent)
    : SettingsWidgetBase{account, parent}
    , mEnableE2E(new QCheckBox(i18n("Enabled E2E encryption"), this))
{
    auto mainLayout = new QFormLayout(this);
    mainLayout->setObjectName(QStringLiteral("mainLayout"));
    mainLayout->setContentsMargins({});

    mEnableE2E->setObjectName(QStringLiteral("mEnableE2E"));
    mEnableE2E->setToolTip(i18n("Enable option to create encrypted groups and be able to change groups and direct messages to be encrypted."));
    mainLayout->addWidget(mEnableE2E);
    connect(mEnableE2E, &QCheckBox::clicked, this, [this](bool checked) {
        updateSettings(QStringLiteral("E2E_Enable"), checked);
    });
}

EncryptionSettingsWidget::~EncryptionSettingsWidget() = default;

void EncryptionSettingsWidget::initialize()
{
    // TODO
}
