/*
   SPDX-FileCopyrightText: 2022-2026 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "mobilesettingswidget.h"
using namespace Qt::Literals::StringLiterals;

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
    mAllowSaveMediaGallery->setObjectName(u"mAllowSaveMediaGallery"_s);
    addCheckBox(mAllowSaveMediaGallery, u"Allow_Save_Media_to_Gallery"_s);

    auto screenLockLabel = createBoldLabel(i18n("Screen Lock"));
    screenLockLabel->setObjectName(u"screenLockLabel"_s);
    mMainLayout->addWidget(screenLockLabel);

    mForceScreenLock->setObjectName(u"mForceScreenLock"_s);
    mForceScreenLock->setToolTip(i18nc("@info:tooltip", "When enabled, you'll force your users to use a PIN/BIOMETRY/FACEID to unlock the app."));
    addCheckBox(mForceScreenLock, u"Force_Screen_Lock"_s);

    mForceScreenLockAfter->setObjectName(u"mForceScreenLockAfter"_s);
    mForceScreenLockAfter->setMaximum(99999);
    mForceScreenLockAfter->setToolTip(i18nc("@info:tooltip", "The time to request password again after the finish of the latest session, in seconds."));
    addSpinbox(i18n("Force Screen Lock After"), mForceScreenLockAfter, u"Force_Screen_Lock_After"_s);
}

MobileSettingsWidget::~MobileSettingsWidget() = default;

void MobileSettingsWidget::initialize(const QMap<QString, SettingsWidgetBase::SettingsInfo> &mapSettings)
{
    initializeWidget(mAllowSaveMediaGallery, mapSettings, true);
    initializeWidget(mForceScreenLock, mapSettings, true);
    initializeWidget(mForceScreenLockAfter, mapSettings, 1800);
}

#include "moc_mobilesettingswidget.cpp"
