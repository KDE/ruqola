/*
   SPDX-FileCopyrightText: 2022-2025 Laurent Montel <montel@kde.org>

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
    , mAllowSaveMediaGallery(new QCheckBox(i18nc("@option:check", "Allow Save Media to Gallery"), this))
    , mForceScreenLock(new QCheckBox(i18nc("@option:check", "Force screen lock"), this))
    , mForceScreenLockAfter(new QSpinBox(this))
{
    mAllowSaveMediaGallery->setObjectName(QStringLiteral("mAllowSaveMediaGallery"));
    addCheckBox(mAllowSaveMediaGallery, QStringLiteral("Allow_Save_Media_to_Gallery"));

    auto screenLockLabel = createBoldLabel(i18n("Screen Lock"));
    screenLockLabel->setObjectName(QStringLiteral("screenLockLabel"));
    mMainLayout->addWidget(screenLockLabel);

    mForceScreenLock->setObjectName(QStringLiteral("mForceScreenLock"));
    mForceScreenLock->setToolTip(i18nc("@info:tooltip", "When enabled, you'll force your users to use a PIN/BIOMETRY/FACEID to unlock the app."));
    addCheckBox(mForceScreenLock, QStringLiteral("Force_Screen_Lock"));

    mForceScreenLockAfter->setObjectName(QStringLiteral("mForceScreenLockAfter"));
    mForceScreenLockAfter->setMaximum(99999);
    mForceScreenLockAfter->setToolTip(i18nc("@info:tooltip", "The time to request password again after the finish of the latest session, in seconds."));
    addSpinbox(i18n("Force Screen Lock After"), mForceScreenLockAfter, QStringLiteral("Force_Screen_Lock_After"));
}

MobileSettingsWidget::~MobileSettingsWidget() = default;

void MobileSettingsWidget::initialize(const QMap<QString, SettingsWidgetBase::SettingsInfo> &mapSettings)
{
    initializeWidget(mAllowSaveMediaGallery, mapSettings, true);
    initializeWidget(mForceScreenLock, mapSettings, true);
    initializeWidget(mForceScreenLockAfter, mapSettings, 1800);
}

#include "moc_mobilesettingswidget.cpp"
