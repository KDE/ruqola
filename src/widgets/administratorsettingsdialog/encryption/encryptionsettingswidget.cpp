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
    mEnableE2E->setObjectName(QStringLiteral("mEnableE2E"));
    mEnableE2E->setToolTip(i18n("Enable option to create encrypted groups and be able to change groups and direct messages to be encrypted."));
    mMainLayout->addWidget(mEnableE2E);
    connectCheckBox(mEnableE2E, QStringLiteral("E2E_Enable"));

    mEnableEncryptionDirectRoomsByDefault->setObjectName(QStringLiteral("mEnableEncryptionDirectRoomsByDefault"));
    mMainLayout->addWidget(mEnableEncryptionDirectRoomsByDefault);
    connectCheckBox(mEnableEncryptionDirectRoomsByDefault, QStringLiteral("E2E_Enabled_Default_DirectRooms"));

    mEnableEncryptionPrivateRoomsByDefault->setObjectName(QStringLiteral("mEnableEncryptionPrivateRoomsByDefault"));
    mMainLayout->addWidget(mEnableEncryptionPrivateRoomsByDefault);
    connectCheckBox(mEnableEncryptionPrivateRoomsByDefault, QStringLiteral("E2E_Enabled_Default_PrivateRooms"));

    mEnableOtr->setObjectName(QStringLiteral("mEnableOtr"));
    mEnableOtr->setToolTip(
        i18n("Enable option to use off-the-record (OTR) messages in direct messages between 2 users. OTR messages are not recorded on the server and exchanged "
             "directly and encrypted between the 2 users."));
    mMainLayout->addWidget(mEnableOtr);
    connectCheckBox(mEnableOtr, QStringLiteral("OTR_Enabled"));
}

EncryptionSettingsWidget::~EncryptionSettingsWidget() = default;

void EncryptionSettingsWidget::initialize(const QMap<QString, QVariant> &mapSettings)
{
    // TODO
}
