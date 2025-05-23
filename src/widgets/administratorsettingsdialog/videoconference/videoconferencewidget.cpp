/*
   SPDX-FileCopyrightText: 2022-2025 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "videoconferencewidget.h"

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
    auto bigBlueButtonLabel = createBoldLabel(QStringLiteral("BigBlueButton"));
    bigBlueButtonLabel->setObjectName(QStringLiteral("bigBlueButtonLabel"));
    mMainLayout->addWidget(bigBlueButtonLabel);

    mBigBlueEnabled->setObjectName(QStringLiteral("mBigBlueEnabled"));
    mMainLayout->addWidget(mBigBlueEnabled);
    connectCheckBox(mBigBlueEnabled, QStringLiteral("bigbluebutton_Enabled"));

    mServer->setObjectName(QStringLiteral("mServer"));
    addLineEdit(i18n("Domain"), mServer, QStringLiteral("bigbluebutton_server"));

    mSharedSecret->setObjectName(QStringLiteral("mSharedSecret"));
    addLineEdit(i18n("Shared Secret"), mSharedSecret, QStringLiteral("bigbluebutton_sharedSecret"));

    mAlwaysOpenNewWindow->setObjectName(QStringLiteral("mAlwaysOpenNewWindow"));
    mMainLayout->addWidget(mAlwaysOpenNewWindow);
    connectCheckBox(mAlwaysOpenNewWindow, QStringLiteral("bigbluebutton_Open_New_Window"));

    auto jitsiButtonLabel = createBoldLabel(QStringLiteral("Jitsi"));
    jitsiButtonLabel->setObjectName(QStringLiteral("jitsiButtonLabel"));
    mMainLayout->addWidget(jitsiButtonLabel);

    mJitsiEnabled->setObjectName(QStringLiteral("mJitsiEnabled"));
    mMainLayout->addWidget(mJitsiEnabled);
    connectCheckBox(mJitsiEnabled, QStringLiteral("Jitsi_Enabled"));

    mJitsiDomain->setObjectName(QStringLiteral("mJitsiDomain"));
    addLineEdit(i18n("Domain"), mJitsiDomain, QStringLiteral("Jitsi_Domain"));

    mJitsiRoomPrefix->setObjectName(QStringLiteral("mJitsiRoomPrefix"));
    addLineEdit(i18n("URL room prefix"), mJitsiRoomPrefix, QStringLiteral("Jitsi_URL_Room_Prefix"));

    mJitsiRoomSuffix->setObjectName(QStringLiteral("mJitsiRoomSuffix"));
    addLineEdit(i18n("URL room suffix"), mJitsiRoomSuffix, QStringLiteral("Jitsi_URL_Room_Suffix"));

    mJitsiEnableRoomNameHash->setObjectName(QStringLiteral("mJitsiEnableRoomNameHash"));
    mJitsiEnableRoomNameHash->setToolTip(i18nc("@info:tooltip", "Recommended to enable if the Jitsi instance doesn't use any authentication mechanism."));
    mMainLayout->addWidget(mJitsiEnableRoomNameHash);
    connectCheckBox(mJitsiEnableRoomNameHash, QStringLiteral("Jitsi_URL_Room_Hash"));

    mJitsiUseSSL->setObjectName(QStringLiteral("mJitsiUseSSL"));
    mMainLayout->addWidget(mJitsiUseSSL);
    connectCheckBox(mJitsiUseSSL, QStringLiteral("Jitsi_SSL"));

    mJitsiAlwaysOpenNewWindow->setObjectName(QStringLiteral("mJitsiAlwaysOpenNewWindow"));
    mMainLayout->addWidget(mJitsiAlwaysOpenNewWindow);
    connectCheckBox(mJitsiAlwaysOpenNewWindow, QStringLiteral("Jitsi_Open_New_Window"));

    mJitsiEnableInChannels->setObjectName(QStringLiteral("mJitsiEnableInChannels"));
    mMainLayout->addWidget(mJitsiEnableInChannels);
    connectCheckBox(mJitsiEnableInChannels, QStringLiteral("Jitsi_Enable_Channels"));

    mJitsiEnableTeams->setObjectName(QStringLiteral("mJitsiEnableTeams"));
    mMainLayout->addWidget(mJitsiEnableTeams);
    connectCheckBox(mJitsiEnableTeams, QStringLiteral("Jitsi_Enable_Teams"));

    mJitsiChromeExtensionId->setObjectName(QStringLiteral("mJitsiChromeExtensionId"));
    addLineEdit(i18n("Chrome Extension Id"), mJitsiChromeExtensionId, QStringLiteral("Jitsi_Chrome_Extension"));

    mJitsiEnableJWTAuth->setObjectName(QStringLiteral("mJitsiEnableJWTAuth"));
    mMainLayout->addWidget(mJitsiEnableJWTAuth);
    connectCheckBox(mJitsiEnableJWTAuth, QStringLiteral("Jitsi_Enabled_TokenAuth"));

    mJitsiApplicationID->setObjectName(QStringLiteral("mJitsiApplicationID"));
    addLineEdit(i18n("Application ID (iss)"), mJitsiApplicationID, QStringLiteral("Jitsi_Application_ID"));

    mJitsiApplicationSecret->setObjectName(QStringLiteral("mJitsiApplicationSecret"));
    addLineEdit(i18n("Application Secret"), mJitsiApplicationSecret, QStringLiteral("Jitsi_Application_Secret"));

    mJitsiLimitTokenJitsiRoom->setObjectName(QStringLiteral("mJitsiLimitTokenJitsiRoom"));
    mMainLayout->addWidget(mJitsiLimitTokenJitsiRoom);
    connectCheckBox(mJitsiLimitTokenJitsiRoom, QStringLiteral("Jitsi_Limit_Token_To_Room"));
}

VideoConferenceWidget::~VideoConferenceWidget() = default;

void VideoConferenceWidget::initialize(const QMap<QString, QVariant> &mapSettings)
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
