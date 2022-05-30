/*
   SPDX-FileCopyrightText: 2022 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "generalsettingswidget.h"

#include <KLocalizedString>

#include <QCheckBox>
#include <QFormLayout>

GeneralSettingsWidget::GeneralSettingsWidget(RocketChatAccount *account, QWidget *parent)
    : SettingsWidgetBase(account, parent)
    , mEnableFavoriteRooms(new QCheckBox(i18n("Enable Favorite Rooms"), this))
{
    auto mainLayout = new QFormLayout(mCurrentWidget);
    mainLayout->setObjectName(QStringLiteral("mainLayout"));
    mainLayout->setContentsMargins({});

    mEnableFavoriteRooms->setObjectName(QStringLiteral("mEnableFavoriteRooms"));
    mainLayout->addWidget(mEnableFavoriteRooms);
    connectCheckBox(mEnableFavoriteRooms, QStringLiteral("Favorite_Rooms"));
}

GeneralSettingsWidget::~GeneralSettingsWidget() = default;

void GeneralSettingsWidget::initialize()
{
    // TODO
}
