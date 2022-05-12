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
    , mEnableEncryptionDirectRoomsByDefault(new QCheckBox(i18n("Enable encryption for Direct Rooms by default"), this))
    , mEnableEncryptionPrivateRoomsByDefault(new QCheckBox(i18n("Enable encryption for Private Rooms by default"), this))
    , mEnableOtr(new QCheckBox(i18n("Enable OTR"), this))
{
    auto mainLayout = new QFormLayout(this);
    mainLayout->setObjectName(QStringLiteral("mainLayout"));
    mainLayout->setContentsMargins({});

    mEnableE2E->setObjectName(QStringLiteral("mEnableE2E"));
    mEnableE2E->setToolTip(i18n("Enable option to create encrypted groups and be able to change groups and direct messages to be encrypted."));
    mainLayout->addWidget(mEnableE2E);
    connectCheckBox(mEnableE2E, QStringLiteral("E2E_Enable"));

    mEnableEncryptionDirectRoomsByDefault->setObjectName(QStringLiteral("mEnableEncryptionDirectRoomsByDefault"));
    mainLayout->addWidget(mEnableEncryptionDirectRoomsByDefault);
    connectCheckBox(mEnableEncryptionDirectRoomsByDefault, QStringLiteral("E2E_Enabled_Default_DirectRooms"));

    mEnableEncryptionPrivateRoomsByDefault->setObjectName(QStringLiteral("mEnableEncryptionPrivateRoomsByDefault"));
    mainLayout->addWidget(mEnableEncryptionPrivateRoomsByDefault);
    connectCheckBox(mEnableEncryptionPrivateRoomsByDefault, QStringLiteral("E2E_Enabled_Default_PrivateRooms"));

    mEnableOtr->setObjectName(QStringLiteral("mEnableOtr"));
    mainLayout->addWidget(mEnableOtr);
    connectCheckBox(mEnableOtr, QStringLiteral("OTR_Enabled"));
}

EncryptionSettingsWidget::~EncryptionSettingsWidget() = default;

void EncryptionSettingsWidget::initialize()
{
    // TODO
}
