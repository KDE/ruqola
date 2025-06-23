/*
   SPDX-FileCopyrightText: 2020-2025 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "configurenotificationwidget.h"
using namespace Qt::Literals::StringLiterals;

#include "misc/soundconfigurewidget.h"
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
    , mDisableNotification(new QCheckBox(i18nc("@option:check", "Disable Notification"), this))
    , mHideUnreadRoomStatus(new QCheckBox(i18nc("@option:check", "Hide Unread Room Status"), this))
    , mMuteGroupMentions(new QCheckBox(i18nc("@option:check", "Mute %1 and %2 mentions", u"@all"_s, u"@here"_s), this))
    , mShowBadgeMentions(new QCheckBox(i18nc("@option:check", "Show badge for mentions"), this))
    , mDesktopAlertCombobox(new QComboBox(this))
    , mDesktopSoundConfigureWidget(new SoundConfigureWidget(account, this))
    , mMobileAlertCombobox(new QComboBox(this))
    , mEmailAlertCombobox(new QComboBox(this))
    , mRocketChatAccount(account)
{
    auto topLayout = new QVBoxLayout(this);
    topLayout->setObjectName(u"topLayout"_s);
    topLayout->setContentsMargins({});

    mDisableNotification->setObjectName(u"mDisableNotification"_s);
    mDisableNotification->setToolTip(i18nc("@info:tooltip", "Receive alerts"));
    topLayout->addWidget(mDisableNotification);
    connect(mDisableNotification, &QCheckBox::clicked, this, [this](bool checked) {
        mRocketChatAccount->changeNotificationsSettings(mRoom->roomId(), RocketChatAccount::NotificationOptionsType::DisableNotifications, checked);
    });

    mHideUnreadRoomStatus->setObjectName(u"mHideUnreadRoomStatus"_s);
    topLayout->addWidget(mHideUnreadRoomStatus);
    connect(mHideUnreadRoomStatus, &QCheckBox::clicked, this, [this](bool checked) {
        mRocketChatAccount->changeNotificationsSettings(mRoom->roomId(), RocketChatAccount::NotificationOptionsType::HideUnreadStatus, checked);
    });

    mMuteGroupMentions->setObjectName(u"mMuteGroupMentions"_s);
    topLayout->addWidget(mMuteGroupMentions);
    connect(mMuteGroupMentions, &QCheckBox::clicked, this, [this](bool checked) {
        mRocketChatAccount->changeNotificationsSettings(mRoom->roomId(), RocketChatAccount::NotificationOptionsType::MuteGroupMentions, checked);
    });

    mShowBadgeMentions->setObjectName(u"mShowBadgeMentions"_s);
    mShowBadgeMentions->setToolTip(i18nc("@info:tooltip", "Display badge for direct mentions only"));
    topLayout->addWidget(mShowBadgeMentions);
    connect(mShowBadgeMentions, &QCheckBox::clicked, this, [this](bool checked) {
        mRocketChatAccount->changeNotificationsSettings(mRoom->roomId(), RocketChatAccount::NotificationOptionsType::HideMentionStatus, !checked);
    });

    auto desktopGroupBox = new QGroupBox(i18n("Desktop"), this);
    desktopGroupBox->setObjectName(u"desktopGroupBox"_s);
    topLayout->addWidget(desktopGroupBox);

    auto desktopGroupBoxLayout = new QFormLayout(desktopGroupBox);
    desktopGroupBoxLayout->setObjectName(u"desktopGroupBoxLayout"_s);

    mDesktopAlertCombobox->setObjectName(u"mDesktopAlertCombobox"_s);
    desktopGroupBoxLayout->addRow(i18n("Alert:"), mDesktopAlertCombobox);
    if (mRocketChatAccount) {
        mDesktopAlertCombobox->setModel(mRocketChatAccount->notificationPreferences()->desktopNotificationModel());
    }
    connect(mDesktopAlertCombobox, &QComboBox::activated, this, [this](int index) {
        mRocketChatAccount->changeNotificationsSettings(mRoom->roomId(),
                                                        RocketChatAccount::NotificationOptionsType::DesktopNotifications,
                                                        mRocketChatAccount->notificationPreferences()->desktopNotificationModel()->currentPreference(index));
    });

    auto soundLayout = new QHBoxLayout;
    soundLayout->setContentsMargins({});
    soundLayout->setSpacing(0);
    soundLayout->addWidget(mDesktopSoundConfigureWidget);

    mDesktopSoundConfigureWidget->setObjectName(u"mDesktopSoundConfigureWidget"_s);
    desktopGroupBoxLayout->addRow(i18n("Sound:"), soundLayout);
    if (mRocketChatAccount) {
        mDesktopSoundConfigureWidget->setSoundModel(mRocketChatAccount->notificationPreferences()->desktopSoundNotificationModel());
    }

    connect(mDesktopSoundConfigureWidget, &SoundConfigureWidget::soundChanged, this, [this](const QByteArray &identifier) {
        mRocketChatAccount->changeNotificationsSettings(mRoom->roomId(), RocketChatAccount::NotificationOptionsType::DesktopSoundNotifications, identifier);
    });

    auto mobileGroupBox = new QGroupBox(i18n("Mobile"), this);
    mobileGroupBox->setObjectName(u"mobileGroupBox"_s);
    topLayout->addWidget(mobileGroupBox);

    auto mobileGroupBoxLayout = new QFormLayout(mobileGroupBox);
    mobileGroupBoxLayout->setObjectName(u"mobileGroupBoxLayout"_s);

    mMobileAlertCombobox->setObjectName(u"mMobileAlertCombobox"_s);
    mobileGroupBoxLayout->addRow(i18n("Alert:"), mMobileAlertCombobox);
    if (mRocketChatAccount) {
        mMobileAlertCombobox->setModel(mRocketChatAccount->notificationPreferences()->mobileNotificationModel());
    }
    connect(mMobileAlertCombobox, &QComboBox::activated, this, [this](int index) {
        mRocketChatAccount->changeNotificationsSettings(mRoom->roomId(),
                                                        RocketChatAccount::NotificationOptionsType::MobilePushNotifications,
                                                        mRocketChatAccount->notificationPreferences()->mobileNotificationModel()->currentPreference(index));
    });

    auto emailGroupBox = new QGroupBox(i18n("Email"), this);
    emailGroupBox->setObjectName(u"emailGroupBox"_s);
    topLayout->addWidget(emailGroupBox);

    auto emailGroupBoxLayout = new QFormLayout(emailGroupBox);
    emailGroupBoxLayout->setObjectName(u"emailGroupBoxLayout"_s);

    mEmailAlertCombobox->setObjectName(u"mEmailAlertCombobox"_s);
    emailGroupBoxLayout->addRow(i18n("Alert:"), mEmailAlertCombobox);
    if (mRocketChatAccount) {
        mEmailAlertCombobox->setModel(mRocketChatAccount->notificationPreferences()->emailNotificationModel());
    }
    connect(mEmailAlertCombobox, &QComboBox::activated, this, [this](int index) {
        mRocketChatAccount->changeNotificationsSettings(mRoom->roomId(),
                                                        RocketChatAccount::NotificationOptionsType::EmailNotifications,
                                                        mRocketChatAccount->notificationPreferences()->emailNotificationModel()->currentPreference(index));
    });
}

ConfigureNotificationWidget::~ConfigureNotificationWidget() = default;

Room *ConfigureNotificationWidget::room() const
{
    return mRoom;
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
    mDesktopSoundConfigureWidget->setCurrentSound(notificationOptions.audioNotificationValue());
    mMobileAlertCombobox->setCurrentIndex(mRocketChatAccount->notificationPreferences()->mobileNotificationModel()->setCurrentNotificationPreference(
        notificationOptions.mobilePushNotification().currentValue()));
    mEmailAlertCombobox->setCurrentIndex(mRocketChatAccount->notificationPreferences()->emailNotificationModel()->setCurrentNotificationPreference(
        notificationOptions.emailNotifications().currentValue()));
    mDesktopSoundConfigureWidget->updateButtonState();
}

#include "moc_configurenotificationwidget.cpp"
