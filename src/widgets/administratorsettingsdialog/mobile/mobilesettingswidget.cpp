/*
   SPDX-FileCopyrightText: 2022 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "mobilesettingswidget.h"
#include <KLocalizedString>
#include <QCheckBox>
#include <QFormLayout>

MobileSettingsWidget::MobileSettingsWidget(RocketChatAccount *account, QWidget *parent)
    : SettingsWidgetBase(account, parent)
    , mAllowSaveMediaGallery(new QCheckBox(i18n("Allow Save Media to Gallery"), this))
{
    mAllowSaveMediaGallery->setObjectName(QStringLiteral("mAllowSaveMediaGallery"));
    mMainLayout->addWidget(mAllowSaveMediaGallery);
    connectCheckBox(mAllowSaveMediaGallery, QStringLiteral("Allow_Save_Media_to_Gallery"));
}

MobileSettingsWidget::~MobileSettingsWidget() = default;

void MobileSettingsWidget::initialize(const QMap<QString, QVariant> &mapSettings)
{
    initializeWidget(mAllowSaveMediaGallery, mapSettings, true);
}
