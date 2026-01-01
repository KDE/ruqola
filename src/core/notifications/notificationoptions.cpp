/*
   SPDX-FileCopyrightText: 2018-2026 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "notificationoptions.h"

#include "ruqola_debug.h"
#include <QJsonObject>

using namespace Qt::Literals::StringLiterals;
NotificationOptions::NotificationOptions() = default;

void NotificationOptions::updateNotificationOptions(const QJsonObject &obj)
{
    parseNotificationOptions(obj);
}

void NotificationOptions::parseNotificationOptions(const QJsonObject &obj)
{
    // qDebug() << " parseNotificationOptions " << obj;
    mHideUnreadStatus = obj.value("hideUnreadStatus"_L1).toBool();
    mHideMentionStatus = obj.value("hideMentionStatus"_L1).toBool();
    mDisableNotifications = obj.value("disableNotifications"_L1).toBool();

    mAudioNotificationValue = obj.value("audioNotificationValue"_L1).toString().toLatin1();

    //"desktopNotificationDuration":0,"desktopNotifications":"mentions"
    mDesktopNotifications =
        NotificationValue{obj.value("desktopNotifications"_L1).toString().toLatin1(), obj.value("desktopPrefOrigin"_L1).toString().toLatin1()};
    //"mobilePushNotifications":"nothing"
    mMobilePushNotification =
        NotificationValue{obj.value("mobilePushNotifications"_L1).toString().toLatin1(), obj.value("mobilePrefOrigin"_L1).toString().toLatin1()};
    //"emailNotifications":"default"
    mEmailNotifications = NotificationValue{obj.value("emailNotifications"_L1).toString().toLatin1(), obj.value("emailPrefOrigin"_L1).toString().toLatin1()};
    //"unreadAlert":"nothing"
    mUnreadTrayIconAlert = obj.value("unreadAlert"_L1).toString();
    mMuteGroupMentions = obj.value("muteGroupMentions"_L1).toBool();
}

QByteArray NotificationOptions::audioNotificationValue() const
{
    return mAudioNotificationValue;
}

void NotificationOptions::setAudioNotificationValue(const QByteArray &newAudioNotificationValue)
{
    mAudioNotificationValue = newAudioNotificationValue;
}

QJsonObject NotificationOptions::serialize(const NotificationOptions &options)
{
    QJsonObject obj;
    if (!options.audioNotificationValue().isEmpty()) {
        obj["audioNotificationValue"_L1] = QString::fromLatin1(options.audioNotificationValue());
    }
    if (options.disableNotifications()) {
        obj["disableNotifications"_L1] = true;
    }
    obj["desktopNotifications"_L1] = QString::fromLatin1(options.desktopNotifications().currentValue());
    obj["mobilePushNotifications"_L1] = QString::fromLatin1(options.mobilePushNotification().currentValue());
    obj["emailNotifications"_L1] = QString::fromLatin1(options.emailNotifications().currentValue());
    if (!options.unreadTrayIconAlert().isEmpty()) {
        obj["unreadAlert"_L1] = options.unreadTrayIconAlert();
    }
    if (options.hideUnreadStatus()) {
        obj["hideUnreadStatus"_L1] = true;
    }
    if (options.muteGroupMentions()) {
        obj["muteGroupMentions"_L1] = true;
    }
    if (options.hideMentionStatus()) {
        obj["hideMentionStatus"_L1] = true;
    }
    return obj;
}

NotificationOptions NotificationOptions::deserialize(const QJsonObject &o)
{
    NotificationOptions options;
    options.setAudioNotificationValue(o["audioNotificationValue"_L1].toString().toLatin1());
    options.setDisableNotifications(o["disableNotifications"_L1].toBool(false));
    options.setUnreadTrayIconAlert(o["unreadAlert"_L1].toString());
    options.setHideUnreadStatus(o["hideUnreadStatus"_L1].toBool(false));
    options.setMuteGroupMentions(o["muteGroupMentions"_L1].toBool(false));
    options.setHideMentionStatus(o["hideMentionStatus"_L1].toBool(false));
    options.setDesktopNotifications(NotificationValue(o["desktopNotifications"_L1].toString().toLatin1(), QByteArray()));
    options.setMobilePushNotification(NotificationValue(o["mobilePushNotifications"_L1].toString().toLatin1(), QByteArray()));
    options.setEmailNotifications(NotificationValue(o["emailNotifications"_L1].toString().toLatin1(), QByteArray()));
    return options;
}

bool NotificationOptions::hideUnreadStatus() const
{
    return mHideUnreadStatus;
}

void NotificationOptions::setHideUnreadStatus(bool value)
{
    mHideUnreadStatus = value;
}

bool NotificationOptions::disableNotifications() const
{
    return mDisableNotifications;
}

void NotificationOptions::setDisableNotifications(bool disableNotifications)
{
    mDisableNotifications = disableNotifications;
}

QString NotificationOptions::unreadTrayIconAlert() const
{
    return mUnreadTrayIconAlert;
}

void NotificationOptions::setUnreadTrayIconAlert(const QString &unreadTrayIconAlert)
{
    mUnreadTrayIconAlert = unreadTrayIconAlert;
}

NotificationOptions::NotificationValue NotificationOptions::emailNotifications() const
{
    return mEmailNotifications;
}

void NotificationOptions::setEmailNotifications(const NotificationValue &emailNotifications)
{
    mEmailNotifications = emailNotifications;
}

NotificationOptions::NotificationValue NotificationOptions::mobilePushNotification() const
{
    return mMobilePushNotification;
}

void NotificationOptions::setMobilePushNotification(const NotificationValue &mobilePushNotification)
{
    mMobilePushNotification = mobilePushNotification;
}

NotificationOptions::NotificationValue NotificationOptions::desktopNotifications() const
{
    return mDesktopNotifications;
}

void NotificationOptions::setDesktopNotifications(const NotificationValue &desktopNotifications)
{
    mDesktopNotifications = desktopNotifications;
}

bool NotificationOptions::operator==(const NotificationOptions &other) const
{
    return (mDesktopNotifications == other.desktopNotifications()) && (mMobilePushNotification == other.mobilePushNotification())
        && (mEmailNotifications == other.emailNotifications()) && (mUnreadTrayIconAlert == other.unreadTrayIconAlert())
        && (mDisableNotifications == other.disableNotifications()) && (mHideUnreadStatus == other.hideUnreadStatus())
        && (mAudioNotificationValue == other.audioNotificationValue()) && (mMuteGroupMentions == other.muteGroupMentions())
        && (mHideMentionStatus == other.hideMentionStatus());
}

bool NotificationOptions::operator!=(const NotificationOptions &other) const
{
    return !operator==(other);
}

bool NotificationOptions::muteGroupMentions() const
{
    return mMuteGroupMentions;
}

void NotificationOptions::setMuteGroupMentions(bool muteGroupMentions)
{
    mMuteGroupMentions = muteGroupMentions;
}

bool NotificationOptions::hideMentionStatus() const
{
    return mHideMentionStatus;
}

void NotificationOptions::setHideMentionStatus(bool newHideMentionStatus)
{
    mHideMentionStatus = newHideMentionStatus;
}

QDebug operator<<(QDebug d, const NotificationOptions &t)
{
    d.space() << "mAudioNotificationValue:" << t.audioNotificationValue();
    d.space() << "mDesktopNotifications:" << t.desktopNotifications();
    d.space() << "mMobilePushNotification:" << t.mobilePushNotification();
    d.space() << "mEmailNotifications:" << t.emailNotifications();
    d.space() << "mDisableNotifications:" << t.disableNotifications();
    d.space() << "hideUnreadStatus:" << t.hideUnreadStatus();
    d.space() << "unreadTrayIconAlert:" << t.unreadTrayIconAlert();
    d.space() << "mMuteGroupMentions:" << t.muteGroupMentions();
    d.space() << "mHideMentionStatus:" << t.hideMentionStatus();
    return d;
}

QDebug operator<<(QDebug d, const NotificationOptions::NotificationValue &t)
{
    d.space() << "value" << t.value;
    d.space() << "preferenceOrigin" << t.preferenceOrigin;
    return d;
}

bool NotificationOptions::NotificationValue::operator==(const NotificationValue &other) const
{
    return other.preferenceOrigin == preferenceOrigin && other.value == value;
}

QByteArray NotificationOptions::NotificationValue::currentValue() const
{
    if (preferenceOrigin == "subscription"_ba && !value.isEmpty()) {
        return value;
    }
    // Keep compatibility
    if (preferenceOrigin.isEmpty() && !value.isEmpty()) {
        return value;
    }
    if (preferenceOrigin.isEmpty() && value.isEmpty()) {
        return QByteArray();
    }
    return "default"_ba;
}

bool NotificationOptions::NotificationValue::isEmpty() const
{
    return preferenceOrigin.isEmpty() && value.isEmpty();
}

#include "moc_notificationoptions.cpp"
