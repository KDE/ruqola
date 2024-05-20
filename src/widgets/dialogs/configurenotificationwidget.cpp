/*
   SPDX-FileCopyrightText: 2020-2024 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "configurenotificationwidget.h"
#include "customsound/customsoundsmanager.h"
#include "model/notificationdesktopsoundpreferencemodel.h"
#include "model/notificationpreferencemodel.h"
#include "notifications/notificationpreferences.h"
#include "rocketchataccount.h"
#include "room.h"
#include <KLocalizedString>
#include <QAudioOutput>
#include <QCheckBox>
#include <QComboBox>
#include <QFormLayout>
#include <QGroupBox>
#include <QMediaPlayer>
#include <QToolButton>
#include <QVBoxLayout>

ConfigureNotificationWidget::ConfigureNotificationWidget(RocketChatAccount *account, QWidget *parent)
    : QWidget(parent)
    , mDisableNotification(new QCheckBox(i18n("Disable Notification"), this))
    , mHideUnreadRoomStatus(new QCheckBox(i18n("Hide Unread Room Status"), this))
    , mMuteGroupMentions(new QCheckBox(i18n("Mute %1 and %2 mentions", QStringLiteral("@all"), QStringLiteral("@here")), this))
    , mShowBadgeMentions(new QCheckBox(i18n("Show badge for mentions"), this))
    , mDesktopAlertCombobox(new QComboBox(this))
    , mDesktopSoundCombobox(new QComboBox(this))
    , mMobileAlertCombobox(new QComboBox(this))
    , mEmailAlertCombobox(new QComboBox(this))
    , mPlaySoundToolButton(new QToolButton(this))
    , mRocketChatAccount(account)
{
    auto topLayout = new QVBoxLayout(this);
    topLayout->setObjectName(QStringLiteral("topLayout"));
    topLayout->setContentsMargins({});

    mDisableNotification->setObjectName(QStringLiteral("mDisableNotification"));
    mDisableNotification->setToolTip(i18n("Receive alerts"));
    topLayout->addWidget(mDisableNotification);
    connect(mDisableNotification, &QCheckBox::clicked, this, [this](bool checked) {
        mRocketChatAccount->changeNotificationsSettings(mRoom->roomId(), RocketChatAccount::DisableNotifications, checked);
    });

    mHideUnreadRoomStatus->setObjectName(QStringLiteral("mHideUnreadRoomStatus"));
    topLayout->addWidget(mHideUnreadRoomStatus);
    connect(mHideUnreadRoomStatus, &QCheckBox::clicked, this, [this](bool checked) {
        mRocketChatAccount->changeNotificationsSettings(mRoom->roomId(), RocketChatAccount::HideUnreadStatus, checked);
    });

    mMuteGroupMentions->setObjectName(QStringLiteral("mMuteGroupMentions"));
    topLayout->addWidget(mMuteGroupMentions);
    connect(mMuteGroupMentions, &QCheckBox::clicked, this, [this](bool checked) {
        mRocketChatAccount->changeNotificationsSettings(mRoom->roomId(), RocketChatAccount::MuteGroupMentions, checked);
    });

    mShowBadgeMentions->setObjectName(QStringLiteral("mShowBadgeMentions"));
    mShowBadgeMentions->setToolTip(i18n("Display badge for direct mentions only"));
    topLayout->addWidget(mShowBadgeMentions);
    connect(mShowBadgeMentions, &QCheckBox::clicked, this, [this](bool checked) {
        mRocketChatAccount->changeNotificationsSettings(mRoom->roomId(), RocketChatAccount::HideMentionStatus, !checked);
    });

    auto desktopGroupBox = new QGroupBox(i18n("Desktop"), this);
    desktopGroupBox->setObjectName(QStringLiteral("desktopGroupBox"));
    topLayout->addWidget(desktopGroupBox);

    auto desktopGroupBoxLayout = new QFormLayout(desktopGroupBox);
    desktopGroupBoxLayout->setObjectName(QStringLiteral("desktopGroupBoxLayout"));

    mDesktopAlertCombobox->setObjectName(QStringLiteral("mDesktopAlertCombobox"));
    desktopGroupBoxLayout->addRow(i18n("Alert:"), mDesktopAlertCombobox);
    if (mRocketChatAccount) {
        mDesktopAlertCombobox->setModel(mRocketChatAccount->notificationPreferences()->desktopNotificationModel());
    }
    connect(mDesktopAlertCombobox, &QComboBox::activated, this, [this](int index) {
        mRocketChatAccount->changeNotificationsSettings(mRoom->roomId(),
                                                        RocketChatAccount::DesktopNotifications,
                                                        mRocketChatAccount->notificationPreferences()->desktopNotificationModel()->currentPreference(index));
    });

    auto soundLayout = new QHBoxLayout;
    soundLayout->setContentsMargins({});
    soundLayout->setSpacing(0);
    soundLayout->addWidget(mDesktopSoundCombobox);
    soundLayout->addWidget(mPlaySoundToolButton);
    mPlaySoundToolButton->setIcon(QIcon::fromTheme(QStringLiteral("media-playback-start")));

    mDesktopSoundCombobox->setObjectName(QStringLiteral("mDesktopSoundCombobox"));
    mPlaySoundToolButton->setObjectName(QStringLiteral("mPlaySoundToolButton"));
    mPlaySoundToolButton->setEnabled(false);
    desktopGroupBoxLayout->addRow(i18n("Sound:"), soundLayout);
    connect(mPlaySoundToolButton, &QToolButton::clicked, this, &ConfigureNotificationWidget::slotPlaySound);
    if (mRocketChatAccount) {
        mDesktopSoundCombobox->setModel(mRocketChatAccount->notificationPreferences()->desktopSoundNotificationModel());
    }
    connect(mDesktopSoundCombobox, &QComboBox::activated, this, [this](int index) {
        const QByteArray identifier = mRocketChatAccount->notificationPreferences()->desktopSoundNotificationModel()->currentPreference(index);
        mRocketChatAccount->changeNotificationsSettings(mRoom->roomId(), RocketChatAccount::DesktopSoundNotifications, identifier);
        updateButtonState();
    });

    auto mobileGroupBox = new QGroupBox(i18n("Mobile"), this);
    mobileGroupBox->setObjectName(QStringLiteral("mobileGroupBox"));
    topLayout->addWidget(mobileGroupBox);

    auto mobileGroupBoxLayout = new QFormLayout(mobileGroupBox);
    mobileGroupBoxLayout->setObjectName(QStringLiteral("mobileGroupBoxLayout"));

    mMobileAlertCombobox->setObjectName(QStringLiteral("mMobileAlertCombobox"));
    mobileGroupBoxLayout->addRow(i18n("Alert:"), mMobileAlertCombobox);
    if (mRocketChatAccount) {
        mMobileAlertCombobox->setModel(mRocketChatAccount->notificationPreferences()->mobileNotificationModel());
    }
    connect(mMobileAlertCombobox, &QComboBox::activated, this, [this](int index) {
        mRocketChatAccount->changeNotificationsSettings(mRoom->roomId(),
                                                        RocketChatAccount::MobilePushNotifications,
                                                        mRocketChatAccount->notificationPreferences()->mobileNotificationModel()->currentPreference(index));
    });

    auto emailGroupBox = new QGroupBox(i18n("Email"), this);
    emailGroupBox->setObjectName(QStringLiteral("emailGroupBox"));
    topLayout->addWidget(emailGroupBox);

    auto emailGroupBoxLayout = new QFormLayout(emailGroupBox);
    emailGroupBoxLayout->setObjectName(QStringLiteral("emailGroupBoxLayout"));

    mEmailAlertCombobox->setObjectName(QStringLiteral("mEmailAlertCombobox"));
    emailGroupBoxLayout->addRow(i18n("Alert:"), mEmailAlertCombobox);
    if (mRocketChatAccount) {
        mEmailAlertCombobox->setModel(mRocketChatAccount->notificationPreferences()->emailNotificationModel());
    }
    connect(mEmailAlertCombobox, &QComboBox::activated, this, [this](int index) {
        mRocketChatAccount->changeNotificationsSettings(mRoom->roomId(),
                                                        RocketChatAccount::EmailNotifications,
                                                        mRocketChatAccount->notificationPreferences()->emailNotificationModel()->currentPreference(index));
    });
}

ConfigureNotificationWidget::~ConfigureNotificationWidget() = default;

Room *ConfigureNotificationWidget::room() const
{
    return mRoom;
}

void ConfigureNotificationWidget::updateButtonState()
{
    const QByteArray identifier =
        mRocketChatAccount->notificationPreferences()->desktopSoundNotificationModel()->currentPreference(mDesktopSoundCombobox->currentIndex());
    mPlaySoundToolButton->setEnabled(identifier != QByteArrayLiteral("none"));
}

void ConfigureNotificationWidget::slotPlaySound()
{
    if (mRocketChatAccount) {
        const QByteArray identifier =
            mRocketChatAccount->notificationPreferences()->desktopSoundNotificationModel()->currentPreference(mDesktopSoundCombobox->currentIndex());
        if (!identifier.isEmpty() || identifier != "none") {
            mRocketChatAccount->playSound(identifier);
        }
    }
}

void ConfigureNotificationWidget::setRoom(Room *room)
{
    mRoom = room;
    NotificationOptions notificationOptions = mRoom->notificationOptions();
    mDisableNotification->setChecked(notificationOptions.disableNotifications());
    mHideUnreadRoomStatus->setChecked(notificationOptions.hideUnreadStatus());
    mMuteGroupMentions->setChecked(notificationOptions.muteGroupMentions());
    mShowBadgeMentions->setChecked(!notificationOptions.hideMentionStatus());
    mDesktopAlertCombobox->setCurrentIndex(mRocketChatAccount->notificationPreferences()->desktopNotificationModel()->setCurrentNotificationPreference(
        notificationOptions.desktopNotifications().currentValue()));
    mDesktopSoundCombobox->setCurrentIndex(mRocketChatAccount->notificationPreferences()->desktopSoundNotificationModel()->setCurrentNotificationPreference(
        notificationOptions.audioNotificationValue()));
    mMobileAlertCombobox->setCurrentIndex(mRocketChatAccount->notificationPreferences()->mobileNotificationModel()->setCurrentNotificationPreference(
        notificationOptions.mobilePushNotification().currentValue()));
    mEmailAlertCombobox->setCurrentIndex(mRocketChatAccount->notificationPreferences()->emailNotificationModel()->setCurrentNotificationPreference(
        notificationOptions.emailNotifications().currentValue()));
    updateButtonState();
}

#include "moc_configurenotificationwidget.cpp"
