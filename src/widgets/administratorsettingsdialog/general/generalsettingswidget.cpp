/*
   SPDX-FileCopyrightText: 2022 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "generalsettingswidget.h"

#include <KLocalizedString>

#include <QCheckBox>
#include <QComboBox>
#include <QFormLayout>
#include <QLabel>
#include <QLineEdit>

GeneralSettingsWidget::GeneralSettingsWidget(RocketChatAccount *account, QWidget *parent)
    : SettingsWidgetBase(account, parent)
    , mEnableFavoriteRooms(new QCheckBox(i18n("Enable Favorite Rooms"), this))
    , mSiteUrl(new QLineEdit(this))
    , mSiteName(new QLineEdit(this))
    , mUTF8UsernamesValidation(new QLineEdit(this))
    , mUTF8ChannelNamesValidation(new QLineEdit(this))
    , mFirstChannelAfterLogin(new QLineEdit(this))
    , mDeepLinkUrl(new QLineEdit(this))
    , mCDNPrefix(new QLineEdit(this))
    , mUnreadCount(new QComboBox(this))
    , mUnreadCountDirectMessages(new QComboBox(this))
    , mEnableUpdateChecker(new QCheckBox(i18n("Enable the Update Checker"), this))
{
    mEnableFavoriteRooms->setObjectName(QStringLiteral("mEnableFavoriteRooms"));
    mMainLayout->addWidget(mEnableFavoriteRooms);
    connectCheckBox(mEnableFavoriteRooms, QStringLiteral("Favorite_Rooms"));

    mSiteUrl->setObjectName(QStringLiteral("mSiteUrl"));
    addLineEdit(i18n("Site URL"), mSiteUrl, QStringLiteral("Site_Url"));

    mSiteName->setObjectName(QStringLiteral("mSiteName"));
    addLineEdit(i18n("Site Name"), mSiteName, QStringLiteral("Site_Name"));

    mUTF8UsernamesValidation->setObjectName(QStringLiteral("mUTF8UsernamesValidation"));
    mUTF8UsernamesValidation->setToolTip(i18n("RegExp that will be used to validate usernames"));
    addLineEdit(i18n("UTF8 Usernames Validation"), mUTF8UsernamesValidation, QStringLiteral("UTF8_User_Names_Validation"));

    mUTF8ChannelNamesValidation->setObjectName(QStringLiteral("mUTF8ChannelNamesValidation"));
    mUTF8ChannelNamesValidation->setToolTip(i18n("RegExp that will be used to validate channel names"));
    addLineEdit(i18n("UTF8 Channel Names Validation"), mUTF8ChannelNamesValidation, QStringLiteral("UTF8_Channel_Names_Validation"));

    mFirstChannelAfterLogin->setObjectName(QStringLiteral("mFirstChannelAfterLogin"));
    addLineEdit(i18n("First Channel After Login"), mFirstChannelAfterLogin, QStringLiteral("First_Channel_After_Login"));

    mDeepLinkUrl->setObjectName(QStringLiteral("mDeepLinkUrl"));
    addLineEdit(i18n("Deep Link Url"), mDeepLinkUrl, QStringLiteral("DeepLink_Url"));

    mCDNPrefix->setObjectName(QStringLiteral("mCDNPrefix"));
    addLineEdit(i18n("CDN Prefix"), mCDNPrefix, QStringLiteral("CDN_PREFIX"));

    mUnreadCount->setObjectName(QStringLiteral("mUnreadCount"));
    QMap<QString, QString> maps;
    maps = {
        {QStringLiteral("all_messages"), i18n("All messages")},
        {QStringLiteral("user_mentions_only"), i18n("User mentions only")},
        {QStringLiteral("group_mentions_only"), i18n("Group mentions only")},
        {QStringLiteral("user_and_group_mentions_only"), i18n("User and group mentions only")},
    };
    addComboBox(i18n("Unread Count"), maps, mUnreadCount, QStringLiteral("Unread_Count"));

    mUnreadCountDirectMessages->setObjectName(QStringLiteral("mUnreadCountDirectMessages"));
    maps = {
        {QStringLiteral("all_messages"), i18n("All messages")},
        {QStringLiteral("mentions_only"), i18n("Mentions only")},
    };
    addComboBox(i18n("Unread Count for Direct Messages"), maps, mUnreadCountDirectMessages, QStringLiteral("Unread_Count_DM"));

    auto updateLabel = createBoldLabel(i18n("Update"));
    updateLabel->setObjectName(QStringLiteral("updateLabel"));
    mMainLayout->addWidget(updateLabel);

    mEnableUpdateChecker->setObjectName(QStringLiteral("mEnableUpdateChecker"));
    mMainLayout->addWidget(mEnableUpdateChecker);
    connectCheckBox(mEnableUpdateChecker, QStringLiteral("Update_EnableChecker"));
    mEnableUpdateChecker->setToolTip(i18n(
        "Checks automatically for new updates / important messages from the Rocket.Chat developers and receives notifications when available. The notification "
        "appears once per new version as a clickable banner and as a message from the Rocket.Cat bot, both visible only for administrators."));
}

GeneralSettingsWidget::~GeneralSettingsWidget() = default;

void GeneralSettingsWidget::initialize(const QMap<QString, QVariant> &mapSettings)
{
    initializeWidget(mEnableFavoriteRooms, mapSettings);
    initializeWidget(mSiteUrl, mapSettings);
    initializeWidget(mSiteName, mapSettings);
    initializeWidget(mUTF8UsernamesValidation, mapSettings);
    initializeWidget(mUTF8ChannelNamesValidation, mapSettings);
    initializeWidget(mFirstChannelAfterLogin, mapSettings);
    initializeWidget(mDeepLinkUrl, mapSettings);
    initializeWidget(mCDNPrefix, mapSettings);
    initializeWidget(mUnreadCount, mapSettings);
    initializeWidget(mUnreadCountDirectMessages, mapSettings);
    initializeWidget(mEnableUpdateChecker, mapSettings);
}
