/*
   SPDX-FileCopyrightText: 2024 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "soundconfigurewidget.h"
#include "configuresoundcombobox.h"
#include "model/notificationdesktopsoundpreferencemodel.h"
#include "rocketchataccount.h"
#include <QHBoxLayout>
#include <QToolButton>

SoundConfigureWidget::SoundConfigureWidget(RocketChatAccount *account, QWidget *parent)
    : QWidget{parent}
    , mConfigureSoundComboBox(new ConfigureSoundComboBox(this))
    , mPlaySoundToolButton(new QToolButton(this))
    , mRocketChatAccount(account)
{
    auto mainLayout = new QHBoxLayout(this);
    mainLayout->setContentsMargins({});
    mConfigureSoundComboBox->setObjectName(QStringLiteral("mSoundNewRoomNotification"));

    mainLayout->addWidget(mConfigureSoundComboBox);

    mPlaySoundToolButton->setObjectName(QStringLiteral("mPlaySoundToolButton"));
    mPlaySoundToolButton->setIcon(QIcon::fromTheme(QStringLiteral("media-playback-start")));
    mainLayout->addWidget(mPlaySoundToolButton);
    connect(mPlaySoundToolButton, &QToolButton::clicked, this, &SoundConfigureWidget::slotPlaySound);
}

SoundConfigureWidget::~SoundConfigureWidget() = default;

void SoundConfigureWidget::setSoundModel(NotificationDesktopSoundPreferenceModel *model)
{
    mDesktopSoundModel = model;
    mConfigureSoundComboBox->setModel(model);
}

void SoundConfigureWidget::slotPlaySound()
{
    if (mRocketChatAccount) {
        const QByteArray identifier = mDesktopSoundModel->currentPreference(mConfigureSoundComboBox->currentIndex());
        if (!identifier.isEmpty() || identifier != "none") {
            mRocketChatAccount->playSound(identifier);
        }
    }
}

#include "moc_soundconfigurewidget.cpp"
