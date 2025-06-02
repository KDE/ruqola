/*
   SPDX-FileCopyrightText: 2022-2025 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "encryptionsettingswidget.h"

#include <QCheckBox>
#include <QFormLayout>

#include <KLocalizedString>

EncryptionSettingsWidget::EncryptionSettingsWidget(RocketChatAccount *account, QWidget *parent)
    : SettingsWidgetBase{account, parent}
    , mEnableE2E(new QCheckBox(i18nc("@option:check", "Enabled E2E encryption"), this))
    , mEnableEncryptionDirectRoomsByDefault(new QCheckBox(i18nc("@option:check", "Enable encryption for Direct Rooms by default"), this))
    , mEnableEncryptionPrivateRoomsByDefault(new QCheckBox(i18nc("@option:check", "Enable encryption for Private Rooms by default"), this))
    , mEnableEncryptFiles(new QCheckBox(i18nc("@option:check", "Encrypt files"), this))
    , mAllowUnencryptedMessages(new QCheckBox(i18nc("@option:check", "Access unencrypted content in encrypted rooms"), this))
    , mEnableMentions(new QCheckBox(i18nc("@option:check", "Mentions"), this))
    , mEnableOtr(new QCheckBox(i18nc("@option:check", "Enable OTR"), this))
{
    mEnableE2E->setObjectName(QStringLiteral("mEnableE2E"));
    mEnableE2E->setToolTip(
        i18nc("@info:tooltip", "Enable option to create encrypted groups and be able to change groups and direct messages to be encrypted."));
    addCheckBox(mEnableE2E, QStringLiteral("E2E_Enable"));

    mEnableEncryptionDirectRoomsByDefault->setObjectName(QStringLiteral("mEnableEncryptionDirectRoomsByDefault"));
    addCheckBox(mEnableEncryptionDirectRoomsByDefault, QStringLiteral("E2E_Enabled_Default_DirectRooms"));

    mEnableEncryptionPrivateRoomsByDefault->setObjectName(QStringLiteral("mEnableEncryptionPrivateRoomsByDefault"));
    addCheckBox(mEnableEncryptionPrivateRoomsByDefault, QStringLiteral("E2E_Enabled_Default_PrivateRooms"));

    mEnableEncryptFiles->setObjectName(QStringLiteral("mEnableEncryptFiles"));
    addCheckBox(mEnableEncryptFiles, QStringLiteral("E2E_Enable_Encrypt_Files"));

    mAllowUnencryptedMessages->setObjectName(QStringLiteral("mAllowUnencryptedMessages"));
    mAllowUnencryptedMessages->setToolTip(
        i18nc("@info:tooltip",
              "Allow access to encrypted rooms to people without room encryption keys. They'll be able to see and send unencrypted messages."));
    addCheckBox(mAllowUnencryptedMessages, QStringLiteral("E2E_Allow_Unencrypted_Messages"));

    mEnableMentions->setObjectName(QStringLiteral("mEnableMentions"));
    mEnableMentions->setToolTip(i18nc("@info:tooltip", "Notify people, and highlight user, channel, and team mentions in encrypted content."));
    addCheckBox(mEnableMentions, QStringLiteral("E2E_Enabled_Mentions"));

    mEnableOtr->setObjectName(QStringLiteral("mEnableOtr"));
    mEnableOtr->setToolTip(
        i18n("Enable option to use off-the-record (OTR) messages in direct messages between 2 users. OTR messages are not recorded on the server and exchanged "
             "directly and encrypted between the 2 users."));
    addCheckBox(mEnableOtr, QStringLiteral("OTR_Enabled"));
}

EncryptionSettingsWidget::~EncryptionSettingsWidget() = default;

void EncryptionSettingsWidget::initialize(const QMap<QString, SettingsWidgetBase::SettingsInfo> &mapSettings)
{
    initializeWidget(mEnableE2E, mapSettings, false);
    initializeWidget(mEnableEncryptionDirectRoomsByDefault, mapSettings, false);
    initializeWidget(mEnableEncryptionPrivateRoomsByDefault, mapSettings, false);
    initializeWidget(mEnableEncryptFiles, mapSettings, true);
    initializeWidget(mAllowUnencryptedMessages, mapSettings, true);
    initializeWidget(mEnableMentions, mapSettings, true);
    initializeWidget(mEnableOtr, mapSettings);
}

#include "moc_encryptionsettingswidget.cpp"
