/*
   SPDX-FileCopyrightText: 2020-2022 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "configuregeneralwidget.h"
#include "ruqolaglobalconfig.h"
#include <KLocalizedString>
#include <QCheckBox>
#include <QVBoxLayout>

ConfigureGeneralWidget::ConfigureGeneralWidget(QWidget *parent)
    : QWidget(parent)
    , mSetOnlineForAllAccount(new QCheckBox(i18n("Set accounts online on startup"), this))
    , mShowImageByDefault(new QCheckBox(i18n("Show images by default"), this))
    , mShowRoomAvatar(new QCheckBox(i18n("Show room avatar"), this))
    , mMarkAsReadOnTextClicked(new QCheckBox(i18n("Mark room as read when clicking to write a reply"), this))
    , mEnableSystemTray(new QCheckBox(i18n("Enable system tray icon"), this))
    , mEnableLogging(new QCheckBox(i18n("Enable logging"), this))
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

    mMarkAsReadOnTextClicked->setObjectName(QStringLiteral("mMarkAsReadOnTextClicked"));
    mainLayout->addWidget(mMarkAsReadOnTextClicked);

    mEnableSystemTray->setObjectName(QStringLiteral("mEnableSystemTray"));
    mainLayout->addWidget(mEnableSystemTray);

    mEnableLogging->setObjectName(QStringLiteral("mEnableLogging"));
    mainLayout->addWidget(mEnableLogging);

    mainLayout->addStretch(1);
}

ConfigureGeneralWidget::~ConfigureGeneralWidget() = default;

void ConfigureGeneralWidget::save()
{
    RuqolaGlobalConfig::self()->setSetOnlineAccounts(mSetOnlineForAllAccount->isChecked());
    RuqolaGlobalConfig::self()->setShowImage(mShowImageByDefault->isChecked());
    RuqolaGlobalConfig::self()->setShowRoomAvatar(mShowRoomAvatar->isChecked());
    RuqolaGlobalConfig::self()->setMarkAsReadOnTextClicked(mMarkAsReadOnTextClicked->isChecked());
    RuqolaGlobalConfig::self()->setEnableSystemTray(mEnableSystemTray->isChecked());
    RuqolaGlobalConfig::self()->setEnableLogging(mEnableLogging->isChecked());
    RuqolaGlobalConfig::self()->save();
}

void ConfigureGeneralWidget::load()
{
    mSetOnlineForAllAccount->setChecked(RuqolaGlobalConfig::self()->setOnlineAccounts());
    mShowImageByDefault->setChecked(RuqolaGlobalConfig::self()->showImage());
    mShowRoomAvatar->setChecked(RuqolaGlobalConfig::self()->showRoomAvatar());
    mMarkAsReadOnTextClicked->setChecked(RuqolaGlobalConfig::self()->markAsReadOnTextClicked());
    mEnableSystemTray->setChecked(RuqolaGlobalConfig::self()->enableSystemTray());
    mEnableLogging->setChecked(RuqolaGlobalConfig::self()->enableLogging());
}
