/*
   SPDX-FileCopyrightText: 2024 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "soundconfigurewidget.h"
#include "configuresoundcombobox.h"
#include "model/notificationdesktopsoundpreferencemodel.h"
#include "model/notificationdesktopsoundpreferenceproxymodel.h"
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
    mainLayout->setSpacing(0);
    mConfigureSoundComboBox->setObjectName(QStringLiteral("mSoundNewRoomNotification"));

    mainLayout->addWidget(mConfigureSoundComboBox);

    mPlaySoundToolButton->setObjectName(QStringLiteral("mPlaySoundToolButton"));
    mPlaySoundToolButton->setIcon(QIcon::fromTheme(QStringLiteral("media-playback-start")));
    mPlaySoundToolButton->setEnabled(false);
    mainLayout->addWidget(mPlaySoundToolButton);
    connect(mPlaySoundToolButton, &QToolButton::clicked, this, &SoundConfigureWidget::slotPlaySound);

    connect(mConfigureSoundComboBox, &QComboBox::activated, this, [this](int index) {
        const QByteArray identifier = mDesktopSoundModel->currentPreference(index);
        Q_EMIT soundChanged(identifier);
        updateButtonState();
    });
}

SoundConfigureWidget::~SoundConfigureWidget() = default;

void SoundConfigureWidget::setSoundModel(NotificationDesktopSoundPreferenceModel *model, bool excludeDefaultNone)
{
    mDesktopSoundModel = model;
    if (excludeDefaultNone) {
        auto soundProxyModel = new NotificationDesktopSoundPreferenceProxyModel(this);
        soundProxyModel->setSourceModel(mDesktopSoundModel);
        mConfigureSoundComboBox->setModel(soundProxyModel);
    } else {
        mConfigureSoundComboBox->setModel(model);
    }
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

void SoundConfigureWidget::setCurrentSound(const QByteArray &identifier)
{
    mConfigureSoundComboBox->setCurrentIndex(mDesktopSoundModel->setCurrentNotificationPreference(identifier));
}

void SoundConfigureWidget::updateButtonState()
{
    const QByteArray identifier = mDesktopSoundModel->currentPreference(mConfigureSoundComboBox->currentIndex());
    mPlaySoundToolButton->setEnabled(identifier != QByteArrayLiteral("none"));
}

QByteArray SoundConfigureWidget::identifier() const
{
    return mConfigureSoundComboBox->identifier();
}

#include "moc_soundconfigurewidget.cpp"
