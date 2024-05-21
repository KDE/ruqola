/*
   SPDX-FileCopyrightText: 2024 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "soundconfigurewidget.h"
#include "configuresoundcombobox.h"
#include <QHBoxLayout>
#include <QToolButton>

SoundConfigureWidget::SoundConfigureWidget(QWidget *parent)
    : QWidget{parent}
    , mConfigureSoundComboBox(new ConfigureSoundComboBox(this))
    , mPlaySoundToolButton(new QToolButton(this))
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

void SoundConfigureWidget::setSoundModel(QAbstractItemModel *model)
{
    mConfigureSoundComboBox->setModel(model);
}

void SoundConfigureWidget::slotPlaySound()
{
#if 0
    if (mRocketChatAccount) {
        const QByteArray identifier =
            mRocketChatAccount->notificationPreferences()->desktopSoundNotificationModel()->currentPreference(mDesktopSoundCombobox->currentIndex());
        if (!identifier.isEmpty() || identifier != "none") {
            mRocketChatAccount->playSound(identifier);
        }
    }
#endif
}

#include "moc_soundconfigurewidget.cpp"
