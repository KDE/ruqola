/*
   SPDX-FileCopyrightText: 2022 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "generalsettingswidget.h"

#include <KLocalizedString>

#include <QCheckBox>
#include <QFormLayout>
#include <QLineEdit>

GeneralSettingsWidget::GeneralSettingsWidget(RocketChatAccount *account, QWidget *parent)
    : SettingsWidgetBase(account, parent)
    , mEnableFavoriteRooms(new QCheckBox(i18n("Enable Favorite Rooms"), this))
    , mSiteUrl(new QLineEdit(this))
    , mSiteName(new QLineEdit(this))
    , mUTF8UsernamesValidation(new QLineEdit(this))
    , mUTF8ChannelNamesValidation(new QLineEdit(this))
    , mRochetChatAccount(account)
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
    if (mRochetChatAccount) {
        // TODO
    }
}

GeneralSettingsWidget::~GeneralSettingsWidget() = default;

void GeneralSettingsWidget::initialize(const QMap<QString, QVariant> &mapSettings)
{
    initializeWidget(mEnableFavoriteRooms, mapSettings);
    initializeWidget(mSiteUrl, mapSettings);
    initializeWidget(mSiteName, mapSettings);
    initializeWidget(mUTF8UsernamesValidation, mapSettings);
    initializeWidget(mUTF8ChannelNamesValidation, mapSettings);
}
