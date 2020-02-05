/*
   Copyright (c) 2020 Laurent Montel <montel@kde.org>

   This library is free software; you can redistribute it and/or modify
   it under the terms of the GNU Library General Public License as published
   by the Free Software Foundation; either version 2 of the License or
   ( at your option ) version 3 or, at the discretion of KDE e.V.
   ( which shall act as a proxy as in section 14 of the GPLv3 ), any later version.

   This library is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
   Library General Public License for more details.

   You should have received a copy of the GNU Library General Public License
   along with this library; see the file COPYING.LIB.  If not, write to
   the Free Software Foundation, Inc., 51 Franklin Street, Fifth Floor,
   Boston, MA 02110-1301, USA.
*/

#include "configurenotificationwidget.h"
#include "ruqola.h"
#include "rocketchataccount.h"
#include "notificationoptionswrapper.h"
#include "roomwrapper.h"
#include "notificationpreferences.h"
#include "model/notificationpreferencemodel.h"
#include "model/notificationpreferencemodel.h"
#include "model/notificationdesktopdurationpreferencemodel.h"
#include "model/notificationdesktopsoundpreferencemodel.h"
#include <QVBoxLayout>
#include <KLocalizedString>
#include <QFormLayout>
#include <QCheckBox>
#include <QGroupBox>
#include <QComboBox>

ConfigureNotificationWidget::ConfigureNotificationWidget(QWidget *parent)
    : QWidget(parent)
{
    QVBoxLayout *topLayout = new QVBoxLayout(this);
    topLayout->setObjectName(QStringLiteral("topLayout"));
    topLayout->setContentsMargins(0, 0, 0, 0);

    QFormLayout *mainLayout = new QFormLayout;
    mainLayout->setObjectName(QStringLiteral("mainLayout"));
    mainLayout->setContentsMargins(0, 0, 0, 0);
    topLayout->addLayout(mainLayout);

    mDisableNotification = new QCheckBox(this);
    mDisableNotification->setObjectName(QStringLiteral("mDisableNotification"));
    mainLayout->addRow(i18n("Disable Notification:"), mDisableNotification);
    connect(mDisableNotification, &QCheckBox::clicked, this, [this](bool checked) {
        Ruqola::self()->rocketChatAccount()->changeNotificationsSettings(mRoomWrapper->roomId(), RocketChatAccount::DisableNotifications, checked);
    });

    mHideUnreadRoomStatus = new QCheckBox(this);
    mHideUnreadRoomStatus->setObjectName(QStringLiteral("mHideUnreadRoomStatus"));
    mainLayout->addRow(i18n("Hide Unread Room Status:"), mHideUnreadRoomStatus);
    connect(mHideUnreadRoomStatus, &QCheckBox::clicked, this, [this](bool checked) {
        Ruqola::self()->rocketChatAccount()->changeNotificationsSettings(mRoomWrapper->roomId(), RocketChatAccount::HideUnreadStatus, checked);
    });

    mMuteGroupMentions = new QCheckBox(this);
    mMuteGroupMentions->setObjectName(QStringLiteral("mMuteGroupMentions"));
    mainLayout->addRow(i18n("Mute Group Mentions:"), mMuteGroupMentions);
    connect(mMuteGroupMentions, &QCheckBox::clicked, this, [this](bool checked) {
        Ruqola::self()->rocketChatAccount()->changeNotificationsSettings(mRoomWrapper->roomId(), RocketChatAccount::MuteGroupMentions, checked);
    });

    QGroupBox *desktopGroupBox = new QGroupBox(i18n("Desktop"), this);
    desktopGroupBox->setObjectName(QStringLiteral("desktopGroupBox"));
    topLayout->addWidget(desktopGroupBox);

    QFormLayout *desktopGroupBoxLayout = new QFormLayout(desktopGroupBox);

    mDesktopAlertCombobox = new QComboBox(this);
    mDesktopAlertCombobox->setObjectName(QStringLiteral("mDesktopAlertCombobox"));
    desktopGroupBoxLayout->addRow(i18n("Alert:"), mDesktopAlertCombobox);
    mDesktopAlertCombobox->setModel(NotificationPreferences::self()->desktopNotificationModel());

    mDesktopAudioCombobox = new QComboBox(this);
    mDesktopAudioCombobox->setObjectName(QStringLiteral("mDesktopAudioCombobox"));
    desktopGroupBoxLayout->addRow(i18n("Audio:"), mDesktopAudioCombobox);
    mDesktopAudioCombobox->setModel(NotificationPreferences::self()->desktopAudioNotificationModel());

    mDesktopSoundCombobox = new QComboBox(this);
    mDesktopSoundCombobox->setObjectName(QStringLiteral("mDesktopSoundCombobox"));
    desktopGroupBoxLayout->addRow(i18n("Sound:"), mDesktopSoundCombobox);
    mDesktopSoundCombobox->setModel(NotificationPreferences::self()->desktopSoundNotificationModel());

    mDesktopDurationCombobox = new QComboBox(this);
    mDesktopDurationCombobox->setObjectName(QStringLiteral("mDesktopDurationCombobox"));
    desktopGroupBoxLayout->addRow(i18n("Duration:"), mDesktopDurationCombobox);
    mDesktopDurationCombobox->setModel(NotificationPreferences::self()->desktopDurationNotificationModel());


    QGroupBox *mobileGroupBox = new QGroupBox(i18n("Mobile"), this);
    mobileGroupBox->setObjectName(QStringLiteral("mobileGroupBox"));
    topLayout->addWidget(mobileGroupBox);    

    QFormLayout *mobileGroupBoxLayout = new QFormLayout(mobileGroupBox);
    mMobileAlertCombobox = new QComboBox(this);
    mMobileAlertCombobox->setObjectName(QStringLiteral("mMobileAlertCombobox"));
    mobileGroupBoxLayout->addRow(i18n("Alert:"), mMobileAlertCombobox);
    mMobileAlertCombobox->setModel(NotificationPreferences::self()->mobileNotificationModel());


    QGroupBox *emailGroupBox = new QGroupBox(i18n("Email"), this);
    emailGroupBox->setObjectName(QStringLiteral("emailGroupBox"));
    topLayout->addWidget(emailGroupBox);

    QFormLayout *emailGroupBoxLayout = new QFormLayout(emailGroupBox);
    mEmailAlertCombobox = new QComboBox(this);
    mEmailAlertCombobox->setObjectName(QStringLiteral("mEmailAlertCombobox"));
    emailGroupBoxLayout->addRow(i18n("Alert:"), mEmailAlertCombobox);
    mEmailAlertCombobox->setModel(NotificationPreferences::self()->emailNotificationModel());

}

ConfigureNotificationWidget::~ConfigureNotificationWidget()
{
}

RoomWrapper *ConfigureNotificationWidget::roomWrapper() const
{
    return mRoomWrapper;
}

void ConfigureNotificationWidget::setRoomWrapper(RoomWrapper *roomWrapper)
{
    mRoomWrapper = roomWrapper;
}
