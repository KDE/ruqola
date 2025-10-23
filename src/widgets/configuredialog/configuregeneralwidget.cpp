/*
   SPDX-FileCopyrightText: 2020-2025 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "configuregeneralwidget.h"

#include "ruqolaglobalconfig.h"
#include <KLocalizedString>
#include <QCheckBox>
#include <QVBoxLayout>

using namespace Qt::Literals::StringLiterals;
ConfigureGeneralWidget::ConfigureGeneralWidget(QWidget *parent)
    : QWidget(parent)
    , mSetOnlineForAllAccount(new QCheckBox(i18nc("@option:check", "Set status to \"Online\" on startup"), this))
    , mShowImageByDefault(new QCheckBox(i18nc("@option:check", "Enable image previews"), this))
    , mShowPreviewUrlByDefault(new QCheckBox(i18nc("@option:check", "Enable URL previews"), this))
    , mMarkAsReadOnTextClicked(new QCheckBox(i18nc("@option:check", "Mark channel as read when writing a reply"), this))
    , mEnableSystemTray(new QCheckBox(i18nc("@option:check", "Enable system tray icon"), this))
    , mEnableLogging(new QCheckBox(i18nc("@option:check", "Enable logging"), this))
    , mShowHoverHightLights(new QCheckBox(i18nc("@option:check", "Highlight messages when hovering"), this))
    , mAnimateGif(new QCheckBox(i18nc("@option:check", "Animate GIFs"), this))
    , mStoreMessageInDataBase(new QCheckBox(i18nc("@option:check", "Enable experimental local database"), this))
#if HAVE_ACTIVITY_SUPPORT
    , mEnabledActivitySupport(new QCheckBox(i18nc("@option:check", "Enable Plasma Activities integration"), this))
#endif
{
    auto mainLayout = new QVBoxLayout(this);
    mainLayout->setObjectName(u"mainLayout"_s);

    mSetOnlineForAllAccount->setObjectName(u"mSetOnlineForAllAccount"_s);
    mainLayout->addWidget(mSetOnlineForAllAccount);

    mShowImageByDefault->setObjectName(u"mShowImageByDefault"_s);
    mainLayout->addWidget(mShowImageByDefault);

    mShowPreviewUrlByDefault->setObjectName(u"mShowPreviewUrlByDefault"_s);
    mainLayout->addWidget(mShowPreviewUrlByDefault);

    mMarkAsReadOnTextClicked->setObjectName(u"mMarkAsReadOnTextClicked"_s);
    mainLayout->addWidget(mMarkAsReadOnTextClicked);

    mEnableSystemTray->setObjectName(u"mEnableSystemTray"_s);
    mainLayout->addWidget(mEnableSystemTray);

    mEnableLogging->setObjectName(u"mEnableLogging"_s);
    mainLayout->addWidget(mEnableLogging);

    mShowHoverHightLights->setObjectName(u"mShowHoverHightLights"_s);
    mainLayout->addWidget(mShowHoverHightLights);

    mAnimateGif->setObjectName(u"mAnimateGif"_s);
    mainLayout->addWidget(mAnimateGif);

    mStoreMessageInDataBase->setObjectName(u"mStoreMessageInDataBase"_s);
    mainLayout->addWidget(mStoreMessageInDataBase);

#if HAVE_ACTIVITY_SUPPORT
    mEnabledActivitySupport->setObjectName(u"mEnabledActivitySupport"_s);
    mainLayout->addWidget(mEnabledActivitySupport);
#endif

    mainLayout->addStretch(1);
}

ConfigureGeneralWidget::~ConfigureGeneralWidget() = default;

void ConfigureGeneralWidget::save()
{
    RuqolaGlobalConfig::self()->setSetOnlineAccounts(mSetOnlineForAllAccount->isChecked());
    RuqolaGlobalConfig::self()->setShowImage(mShowImageByDefault->isChecked());
    RuqolaGlobalConfig::self()->setMarkAsReadOnTextClicked(mMarkAsReadOnTextClicked->isChecked());
    RuqolaGlobalConfig::self()->setEnableSystemTray(mEnableSystemTray->isChecked());
    RuqolaGlobalConfig::self()->setEnableLogging(mEnableLogging->isChecked());
    RuqolaGlobalConfig::self()->setShowHoverHighlights(mShowHoverHightLights->isChecked());
    RuqolaGlobalConfig::self()->setAnimateGifImage(mAnimateGif->isChecked());
    RuqolaGlobalConfig::self()->setStoreMessageInDataBase(mStoreMessageInDataBase->isChecked());
    RuqolaGlobalConfig::self()->setShowPreviewUrl(mShowPreviewUrlByDefault->isChecked());
#if HAVE_ACTIVITY_SUPPORT
    RuqolaGlobalConfig::self()->setPlasmaActivities(mEnabledActivitySupport->isChecked());
#endif
    RuqolaGlobalConfig::self()->save();
}

void ConfigureGeneralWidget::load()
{
    mSetOnlineForAllAccount->setChecked(RuqolaGlobalConfig::self()->setOnlineAccounts());
    mShowImageByDefault->setChecked(RuqolaGlobalConfig::self()->showImage());
    mMarkAsReadOnTextClicked->setChecked(RuqolaGlobalConfig::self()->markAsReadOnTextClicked());
    mEnableSystemTray->setChecked(RuqolaGlobalConfig::self()->enableSystemTray());
    mEnableLogging->setChecked(RuqolaGlobalConfig::self()->enableLogging());
    mShowHoverHightLights->setChecked(RuqolaGlobalConfig::self()->showHoverHighlights());
    mAnimateGif->setChecked(RuqolaGlobalConfig::self()->animateGifImage());
    mStoreMessageInDataBase->setChecked(RuqolaGlobalConfig::self()->storeMessageInDataBase());
    mShowPreviewUrlByDefault->setChecked(RuqolaGlobalConfig::self()->showPreviewUrl());
#if HAVE_ACTIVITY_SUPPORT
    mEnabledActivitySupport->setChecked(RuqolaGlobalConfig::self()->plasmaActivities());
#endif
}

void ConfigureGeneralWidget::restoreToDefaults()
{
    // TODO
}

#include "moc_configuregeneralwidget.cpp"
