/*
   SPDX-FileCopyrightText: 2022-2026 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "encryptionsettingswidget.h"
using namespace Qt::Literals::StringLiterals;

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
    mEnableE2E->setObjectName(u"mEnableE2E"_s);
    mEnableE2E->setToolTip(
        i18nc("@info:tooltip", "Enable option to create encrypted groups and be able to change groups and direct messages to be encrypted."));
    addCheckBox(mEnableE2E, u"E2E_Enable"_s);

    mEnableEncryptionDirectRoomsByDefault->setObjectName(u"mEnableEncryptionDirectRoomsByDefault"_s);
    addCheckBox(mEnableEncryptionDirectRoomsByDefault, u"E2E_Enabled_Default_DirectRooms"_s);

    mEnableEncryptionPrivateRoomsByDefault->setObjectName(u"mEnableEncryptionPrivateRoomsByDefault"_s);
    addCheckBox(mEnableEncryptionPrivateRoomsByDefault, u"E2E_Enabled_Default_PrivateRooms"_s);

    mEnableEncryptFiles->setObjectName(u"mEnableEncryptFiles"_s);
    addCheckBox(mEnableEncryptFiles, u"E2E_Enable_Encrypt_Files"_s);

    mAllowUnencryptedMessages->setObjectName(u"mAllowUnencryptedMessages"_s);
    mAllowUnencryptedMessages->setToolTip(
        i18nc("@info:tooltip",
              "Allow access to encrypted rooms to people without room encryption keys. They'll be able to see and send unencrypted messages."));
    addCheckBox(mAllowUnencryptedMessages, u"E2E_Allow_Unencrypted_Messages"_s);

    mEnableMentions->setObjectName(u"mEnableMentions"_s);
    mEnableMentions->setToolTip(i18nc("@info:tooltip", "Notify people, and highlight user, channel, and team mentions in encrypted content."));
    addCheckBox(mEnableMentions, u"E2E_Enabled_Mentions"_s);

    mEnableOtr->setObjectName(u"mEnableOtr"_s);
    mEnableOtr->setToolTip(
        i18n("Enable option to use off-the-record (OTR) messages in direct messages between 2 users. OTR messages are not recorded on the server and exchanged "
             "directly and encrypted between the 2 users."));
    addCheckBox(mEnableOtr, u"OTR_Enabled"_s);
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
