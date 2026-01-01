/*
   SPDX-FileCopyrightText: 2022-2026 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "videoconferencewidget.h"
using namespace Qt::Literals::StringLiterals;

#include <KLocalizedString>
#include <QCheckBox>
#include <QFormLayout>
#include <QLabel>
#include <QLineEdit>

VideoConferenceWidget::VideoConferenceWidget(RocketChatAccount *account, QWidget *parent)
    : SettingsWidgetBase(account, parent)
    , mBigBlueEnabled(new QCheckBox(i18nc("@option:check", "Enabled"), this))
    , mServer(new QLineEdit(this))
    , mSharedSecret(new QLineEdit(this))
    , mAlwaysOpenNewWindow(new QCheckBox(i18nc("@option:check", "Always Open in New Window"), this))
    , mJitsiEnabled(new QCheckBox(i18nc("@option:check", "Enabled"), this))
    , mJitsiDomain(new QLineEdit(this))
    , mJitsiRoomPrefix(new QLineEdit(this))
    , mJitsiRoomSuffix(new QLineEdit(this))
    , mJitsiEnableRoomNameHash(new QCheckBox(i18nc("@option:check", "Enable room name hash"), this))
    , mJitsiUseSSL(new QCheckBox(i18nc("@option:check", "Use SSL"), this))
    , mJitsiAlwaysOpenNewWindow(new QCheckBox(i18nc("@option:check", "Always Open in New Window"), this))
    , mJitsiEnableInChannels(new QCheckBox(i18nc("@option:check", "Enable in Channels"), this))
    , mJitsiEnableTeams(new QCheckBox(i18nc("@option:check", "Enable for Teams"), this))
    , mJitsiChromeExtensionId(new QLineEdit(this))
    , mJitsiEnableJWTAuth(new QCheckBox(i18nc("@option:check", "Enable JWT auth"), this))
    , mJitsiApplicationID(new QLineEdit(this))
    , mJitsiApplicationSecret(new QLineEdit(this))
    , mJitsiLimitTokenJitsiRoom(new QCheckBox(i18nc("@option:check", "Limit token to Jitsi Room"), this))
{
    auto bigBlueButtonLabel = createBoldLabel(u"BigBlueButton"_s);
    bigBlueButtonLabel->setObjectName(u"bigBlueButtonLabel"_s);
    mMainLayout->addWidget(bigBlueButtonLabel);

    mBigBlueEnabled->setObjectName(u"mBigBlueEnabled"_s);
    addCheckBox(mBigBlueEnabled, u"bigbluebutton_Enabled"_s);

    mServer->setObjectName(u"mServer"_s);
    addLineEdit(i18n("Domain"), mServer, u"bigbluebutton_server"_s);

    mSharedSecret->setObjectName(u"mSharedSecret"_s);
    addLineEdit(i18n("Shared Secret"), mSharedSecret, u"bigbluebutton_sharedSecret"_s);

    mAlwaysOpenNewWindow->setObjectName(u"mAlwaysOpenNewWindow"_s);
    addCheckBox(mAlwaysOpenNewWindow, u"bigbluebutton_Open_New_Window"_s);

    auto jitsiButtonLabel = createBoldLabel(u"Jitsi"_s);
    jitsiButtonLabel->setObjectName(u"jitsiButtonLabel"_s);
    mMainLayout->addWidget(jitsiButtonLabel);

    mJitsiEnabled->setObjectName(u"mJitsiEnabled"_s);
    addCheckBox(mJitsiEnabled, u"Jitsi_Enabled"_s);

    mJitsiDomain->setObjectName(u"mJitsiDomain"_s);
    addLineEdit(i18n("Domain"), mJitsiDomain, u"Jitsi_Domain"_s);

    mJitsiRoomPrefix->setObjectName(u"mJitsiRoomPrefix"_s);
    addLineEdit(i18n("URL room prefix"), mJitsiRoomPrefix, u"Jitsi_URL_Room_Prefix"_s);

    mJitsiRoomSuffix->setObjectName(u"mJitsiRoomSuffix"_s);
    addLineEdit(i18n("URL room suffix"), mJitsiRoomSuffix, u"Jitsi_URL_Room_Suffix"_s);

    mJitsiEnableRoomNameHash->setObjectName(u"mJitsiEnableRoomNameHash"_s);
    mJitsiEnableRoomNameHash->setToolTip(i18nc("@info:tooltip", "Recommended to enable if the Jitsi instance doesn't use any authentication mechanism."));
    addCheckBox(mJitsiEnableRoomNameHash, u"Jitsi_URL_Room_Hash"_s);

    mJitsiUseSSL->setObjectName(u"mJitsiUseSSL"_s);
    addCheckBox(mJitsiUseSSL, u"Jitsi_SSL"_s);

    mJitsiAlwaysOpenNewWindow->setObjectName(u"mJitsiAlwaysOpenNewWindow"_s);
    addCheckBox(mJitsiAlwaysOpenNewWindow, u"Jitsi_Open_New_Window"_s);

    mJitsiEnableInChannels->setObjectName(u"mJitsiEnableInChannels"_s);
    addCheckBox(mJitsiEnableInChannels, u"Jitsi_Enable_Channels"_s);

    mJitsiEnableTeams->setObjectName(u"mJitsiEnableTeams"_s);
    addCheckBox(mJitsiEnableTeams, u"Jitsi_Enable_Teams"_s);

    mJitsiChromeExtensionId->setObjectName(u"mJitsiChromeExtensionId"_s);
    addLineEdit(i18n("Chrome Extension Id"), mJitsiChromeExtensionId, u"Jitsi_Chrome_Extension"_s);

    mJitsiEnableJWTAuth->setObjectName(u"mJitsiEnableJWTAuth"_s);
    addCheckBox(mJitsiEnableJWTAuth, u"Jitsi_Enabled_TokenAuth"_s);

    mJitsiApplicationID->setObjectName(u"mJitsiApplicationID"_s);
    addLineEdit(i18n("Application ID (iss)"), mJitsiApplicationID, u"Jitsi_Application_ID"_s);

    mJitsiApplicationSecret->setObjectName(u"mJitsiApplicationSecret"_s);
    addLineEdit(i18n("Application Secret"), mJitsiApplicationSecret, u"Jitsi_Application_Secret"_s);

    mJitsiLimitTokenJitsiRoom->setObjectName(u"mJitsiLimitTokenJitsiRoom"_s);
    addCheckBox(mJitsiLimitTokenJitsiRoom, u"Jitsi_Limit_Token_To_Room"_s);
}

VideoConferenceWidget::~VideoConferenceWidget() = default;

void VideoConferenceWidget::initialize(const QMap<QString, SettingsWidgetBase::SettingsInfo> &mapSettings)
{
    initializeWidget(mBigBlueEnabled, mapSettings);
    initializeWidget(mServer, mapSettings);
    initializeWidget(mSharedSecret, mapSettings);
    initializeWidget(mAlwaysOpenNewWindow, mapSettings);

    initializeWidget(mJitsiEnabled, mapSettings);
    initializeWidget(mJitsiDomain, mapSettings);
    initializeWidget(mJitsiRoomPrefix, mapSettings);
    initializeWidget(mJitsiRoomSuffix, mapSettings);
    initializeWidget(mJitsiEnableRoomNameHash, mapSettings);
    initializeWidget(mJitsiUseSSL, mapSettings);
    initializeWidget(mJitsiAlwaysOpenNewWindow, mapSettings);
    initializeWidget(mJitsiEnableInChannels, mapSettings);
    initializeWidget(mJitsiEnableTeams, mapSettings);
    initializeWidget(mJitsiChromeExtensionId, mapSettings);
    initializeWidget(mJitsiEnableJWTAuth, mapSettings);
    initializeWidget(mJitsiApplicationID, mapSettings);
    initializeWidget(mJitsiApplicationSecret, mapSettings);
    initializeWidget(mJitsiLimitTokenJitsiRoom, mapSettings);
}

#include "moc_videoconferencewidget.cpp"
