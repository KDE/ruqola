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
{
    mEnableFavoriteRooms->setObjectName(QStringLiteral("mEnableFavoriteRooms"));
    mMainLayout->addWidget(mEnableFavoriteRooms);
    connectCheckBox(mEnableFavoriteRooms, QStringLiteral("Favorite_Rooms"));

    mSiteUrl->setObjectName(QStringLiteral("mSiteUrl"));
    addLineEdit(i18n("Site URL"), mSiteUrl, QStringLiteral("Site_Url"));
}

GeneralSettingsWidget::~GeneralSettingsWidget() = default;

void GeneralSettingsWidget::initialize()
{
    // TODO
}
