/*
   SPDX-FileCopyrightText: 2022-2023 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "mobilesettingswidget.h"
#include <KLocalizedString>
#include <QCheckBox>
#include <QFormLayout>
#include <QLabel>
#include <QSpinBox>

MobileSettingsWidget::MobileSettingsWidget(RocketChatAccount *account, QWidget *parent)
    : SettingsWidgetBase(account, parent)
    , mAllowSaveMediaGallery(new QCheckBox(i18n("Allow Save Media to Gallery"), this))
    , mForceScreenLock(new QCheckBox(i18n("Force screen lock"), this))
    , mForceScreenLockAfter(new QSpinBox(this))
{
    mAllowSaveMediaGallery->setObjectName(QStringLiteral("mAllowSaveMediaGallery"));
    mMainLayout->addWidget(mAllowSaveMediaGallery);
    connectCheckBox(mAllowSaveMediaGallery, QStringLiteral("Allow_Save_Media_to_Gallery"));

    auto screenLockLabel = createBoldLabel(i18n("Screen Lock"));
    screenLockLabel->setObjectName(QStringLiteral("screenLockLabel"));
    mMainLayout->addWidget(screenLockLabel);

    mForceScreenLock->setObjectName(QStringLiteral("mForceScreenLock"));
    mForceScreenLock->setToolTip(i18n("When enabled, you'll force your users to use a PIN/BIOMETRY/FACEID to unlock the app."));
    mMainLayout->addWidget(mForceScreenLock);
    connectCheckBox(mForceScreenLock, QStringLiteral("Force_Screen_Lock"));

    mForceScreenLockAfter->setObjectName(QStringLiteral("mForceScreenLockAfter"));
    mForceScreenLockAfter->setMaximum(99999);
    mForceScreenLockAfter->setToolTip(i18n("The time to request password again after the finish of the latest session, in seconds."));
    addSpinbox(i18n("Force Screen Lock After"), mForceScreenLockAfter, QStringLiteral("Force_Screen_Lock_After"));
}

MobileSettingsWidget::~MobileSettingsWidget() = default;

void MobileSettingsWidget::initialize(const QMap<QString, QVariant> &mapSettings)
{
    initializeWidget(mAllowSaveMediaGallery, mapSettings, true);
    initializeWidget(mForceScreenLock, mapSettings, true);
    initializeWidget(mForceScreenLockAfter, mapSettings, 1800);
}
