/*
   Copyright (c) 2020 Laurent Montel <montel@kde.org>

   This library is free software; you can redistribute it and/or modify
   it under the terms of the GNU Library General Public License as published
   by the Free Software Foundation; either version 2 of the License or
   ( at your option ) version 3 or, at the discretion of KDE e.V.
   ( which shall act as a proxy as in section 14 of the GPLv3 ), any later version.

   This library is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
   Library General Public License for more details.

   You should have received a copy of the GNU Library General Public License
   along with this library; see the file COPYING.LIB.  If not, write to
   the Free Software Foundation, Inc., 51 Franklin Street, Fifth Floor,
   Boston, MA 02110-1301, USA.
*/

#include "configuregeneralwidget.h"
#include <QVBoxLayout>
#include <KLocalizedString>
#include <QCheckBox>
#include "ruqolaglobalconfig.h"

ConfigureGeneralWidget::ConfigureGeneralWidget(QWidget *parent)
    : QWidget(parent)
    , mSetOnlineForAllAccount(new QCheckBox(i18n("Set Online Account on Startup"), this))
    , mShowImageByDefault(new QCheckBox(i18n("Show Image by Default"), this))
    , mShowRoomAvatar(new QCheckBox(i18n("Show Room Avatar"), this))
    , mEnableSystemTray(new QCheckBox(i18n("Enable system tray icon"), this))
{
    auto mainLayout = new QVBoxLayout(this);
    mainLayout->setObjectName(QStringLiteral("mainLayout"));
    mainLayout->setContentsMargins({});

    mSetOnlineForAllAccount->setObjectName(QStringLiteral("mSetOnlineForAllAccount"));
    mainLayout->addWidget(mSetOnlineForAllAccount);

    mShowImageByDefault->setObjectName(QStringLiteral("mShowImageByDefault"));
    mainLayout->addWidget(mShowImageByDefault);

    mShowRoomAvatar->setObjectName(QStringLiteral("mShowRoomAvatar"));
    mainLayout->addWidget(mShowRoomAvatar);

    mEnableSystemTray->setObjectName(QStringLiteral("mEnableSystemTray"));
    mainLayout->addWidget(mEnableSystemTray);

    mainLayout->addStretch(1);
}

ConfigureGeneralWidget::~ConfigureGeneralWidget()
{
}

void ConfigureGeneralWidget::save()
{
    RuqolaGlobalConfig::self()->setSetOnlineAccounts(mSetOnlineForAllAccount->isChecked());
    RuqolaGlobalConfig::self()->setShowImage(mShowImageByDefault->isChecked());
    RuqolaGlobalConfig::self()->setShowRoomAvatar(mShowRoomAvatar->isChecked());
    RuqolaGlobalConfig::self()->setEnableSystemTray(mEnableSystemTray->isChecked());
    RuqolaGlobalConfig::self()->save();
}

void ConfigureGeneralWidget::load()
{
    mSetOnlineForAllAccount->setChecked(RuqolaGlobalConfig::self()->setOnlineAccounts());
    mShowImageByDefault->setChecked(RuqolaGlobalConfig::self()->showImage());
    mShowRoomAvatar->setChecked(RuqolaGlobalConfig::self()->showRoomAvatar());
    mEnableSystemTray->setChecked(RuqolaGlobalConfig::self()->enableSystemTray());
}
