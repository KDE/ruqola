/*
   SPDX-FileCopyrightText: 2020-2023 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "configurenotificationwidget.h"
#include "model/notificationdesktopsoundpreferencemodel.h"
#include "model/notificationpreferencemodel.h"
#include "notifications/notificationpreferences.h"
#include "rocketchataccount.h"
#include "room.h"
#include <KLocalizedString>
#include <QCheckBox>
#include <QComboBox>
#include <QFormLayout>
#include <QGroupBox>
#include <QVBoxLayout>

ConfigureNotificationWidget::ConfigureNotificationWidget(RocketChatAccount *account, QWidget *parent)
    : QWidget(parent)
    , mDisableNotification(new QCheckBox(i18n("Disable Notification"), this))
    , mHideUnreadRoomStatus(new QCheckBox(i18n("Hide Unread Room Status"), this))
    , mMuteGroupMentions(new QCheckBox(i18n("Mute @all and @here mentions"), this))
    , mShowBadgeMentions(new QCheckBox(i18n("Show badge for mentions"), this))
    , mDesktopAlertCombobox(new QComboBox(this))
    , mDesktopSoundCombobox(new QComboBox(this))
    , mMobileAlertCombobox(new QComboBox(this))
    , mEmailAlertCombobox(new QComboBox(this))
{
    auto topLayout = new QVBoxLayout(this);
    topLayout->setObjectName(QStringLiteral("topLayout"));
    topLayout->setContentsMargins({});

    mDisableNotification->setObjectName(QStringLiteral("mDisableNotification"));
    mDisableNotification->setToolTip(i18n("Receive alerts"));
    topLayout->addWidget(mDisableNotification);
    connect(mDisableNotification, &QCheckBox::clicked, this, [this, account](bool checked) {
        account->changeNotificationsSettings(mRoom->roomId(), RocketChatAccount::DisableNotifications, checked);
    });

    mHideUnreadRoomStatus->setObjectName(QStringLiteral("mHideUnreadRoomStatus"));
    topLayout->addWidget(mHideUnreadRoomStatus);
    connect(mHideUnreadRoomStatus, &QCheckBox::clicked, this, [this, account](bool checked) {
        account->changeNotificationsSettings(mRoom->roomId(), RocketChatAccount::HideUnreadStatus, checked);
    });

    mMuteGroupMentions->setObjectName(QStringLiteral("mMuteGroupMentions"));
    topLayout->addWidget(mMuteGroupMentions);
    connect(mMuteGroupMentions, &QCheckBox::clicked, this, [this, account](bool checked) {
        account->changeNotificationsSettings(mRoom->roomId(), RocketChatAccount::MuteGroupMentions, checked);
    });

    mShowBadgeMentions->setObjectName(QStringLiteral("mShowBadgeMentions"));
    mShowBadgeMentions->setToolTip(i18n("Display badge for direct mentions only"));
    topLayout->addWidget(mShowBadgeMentions);
    connect(mShowBadgeMentions, &QCheckBox::clicked, this, [this, account](bool checked) {
        account->changeNotificationsSettings(mRoom->roomId(), RocketChatAccount::HideMentionStatus, !checked);
    });

    auto desktopGroupBox = new QGroupBox(i18n("Desktop"), this);
    desktopGroupBox->setObjectName(QStringLiteral("desktopGroupBox"));
    topLayout->addWidget(desktopGroupBox);

    auto desktopGroupBoxLayout = new QFormLayout(desktopGroupBox);
    desktopGroupBoxLayout->setObjectName(QStringLiteral("desktopGroupBoxLayout"));

    mDesktopAlertCombobox->setObjectName(QStringLiteral("mDesktopAlertCombobox"));
    desktopGroupBoxLayout->addRow(i18n("Alert:"), mDesktopAlertCombobox);
    mDesktopAlertCombobox->setModel(NotificationPreferences::self()->desktopNotificationModel());
    connect(mDesktopAlertCombobox, &QComboBox::activated, this, [this, account](int index) {
        account->changeNotificationsSettings(mRoom->roomId(),
                                             RocketChatAccount::DesktopNotifications,
                                             NotificationPreferences::self()->desktopNotificationModel()->currentPreference(index));
    });

    mDesktopSoundCombobox->setObjectName(QStringLiteral("mDesktopSoundCombobox"));
    desktopGroupBoxLayout->addRow(i18n("Sound:"), mDesktopSoundCombobox);
    mDesktopSoundCombobox->setModel(NotificationPreferences::self()->desktopSoundNotificationModel());
    connect(mDesktopSoundCombobox, &QComboBox::activated, this, [this, account](int index) {
        account->changeNotificationsSettings(mRoom->roomId(),
                                             RocketChatAccount::DesktopSoundNotifications,
                                             NotificationPreferences::self()->desktopSoundNotificationModel()->currentPreference(index));
    });

    auto mobileGroupBox = new QGroupBox(i18n("Mobile"), this);
    mobileGroupBox->setObjectName(QStringLiteral("mobileGroupBox"));
    topLayout->addWidget(mobileGroupBox);

    auto mobileGroupBoxLayout = new QFormLayout(mobileGroupBox);
    mobileGroupBoxLayout->setObjectName(QStringLiteral("mobileGroupBoxLayout"));

    mMobileAlertCombobox->setObjectName(QStringLiteral("mMobileAlertCombobox"));
    mobileGroupBoxLayout->addRow(i18n("Alert:"), mMobileAlertCombobox);
    mMobileAlertCombobox->setModel(NotificationPreferences::self()->mobileNotificationModel());
    connect(mMobileAlertCombobox, &QComboBox::activated, this, [this, account](int index) {
        account->changeNotificationsSettings(mRoom->roomId(),
                                             RocketChatAccount::MobilePushNotifications,
                                             NotificationPreferences::self()->mobileNotificationModel()->currentPreference(index));
    });

    auto emailGroupBox = new QGroupBox(i18n("Email"), this);
    emailGroupBox->setObjectName(QStringLiteral("emailGroupBox"));
    topLayout->addWidget(emailGroupBox);

    auto emailGroupBoxLayout = new QFormLayout(emailGroupBox);
    emailGroupBoxLayout->setObjectName(QStringLiteral("emailGroupBoxLayout"));

    mEmailAlertCombobox->setObjectName(QStringLiteral("mEmailAlertCombobox"));
    emailGroupBoxLayout->addRow(i18n("Alert:"), mEmailAlertCombobox);
    mEmailAlertCombobox->setModel(NotificationPreferences::self()->emailNotificationModel());
    connect(mEmailAlertCombobox, &QComboBox::activated, this, [this, account](int index) {
        account->changeNotificationsSettings(mRoom->roomId(),
                                             RocketChatAccount::EmailNotifications,
                                             NotificationPreferences::self()->emailNotificationModel()->currentPreference(index));
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
    mDesktopAlertCombobox->setCurrentIndex(NotificationPreferences::self()->desktopNotificationModel()->setCurrentNotificationPreference(
        notificationOptions.desktopNotifications().currentValue()));
    mDesktopSoundCombobox->setCurrentIndex(
        NotificationPreferences::self()->desktopSoundNotificationModel()->setCurrentNotificationPreference(notificationOptions.audioNotificationValue()));
    mMobileAlertCombobox->setCurrentIndex(NotificationPreferences::self()->mobileNotificationModel()->setCurrentNotificationPreference(
        notificationOptions.mobilePushNotification().currentValue()));
    mEmailAlertCombobox->setCurrentIndex(
        NotificationPreferences::self()->emailNotificationModel()->setCurrentNotificationPreference(notificationOptions.emailNotifications().currentValue()));
}
