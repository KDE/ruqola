/*
   SPDX-FileCopyrightText: 2018-2024 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "notificationoptions.h"
#include "ruqola_debug.h"
#include <QJsonObject>

NotificationOptions::NotificationOptions() = default;

void NotificationOptions::updateNotificationOptions(const QJsonObject &obj)
{
    parseNotificationOptions(obj);
}

void NotificationOptions::parseNotificationOptions(const QJsonObject &obj)
{
    // qDebug() << " parseNotificationOptions " << obj;
    mHideUnreadStatus = obj.value(QLatin1StringView("hideUnreadStatus")).toBool();
    mHideMentionStatus = obj.value(QLatin1StringView("hideMentionStatus")).toBool();
    mDisableNotifications = obj.value(QLatin1StringView("disableNotifications")).toBool();

    mAudioNotificationValue = obj.value(QLatin1StringView("audioNotificationValue")).toString();

    //"desktopNotificationDuration":0,"desktopNotifications":"mentions"
    mDesktopNotifications = NotificationValue{obj.value(QLatin1StringView("desktopNotifications")).toString().toLatin1(),
                                              obj.value(QLatin1StringView("desktopPrefOrigin")).toString().toLatin1()};
    //"mobilePushNotifications":"nothing"
    mMobilePushNotification = NotificationValue{obj.value(QLatin1StringView("mobilePushNotifications")).toString().toLatin1(),
                                                obj.value(QLatin1StringView("mobilePrefOrigin")).toString().toLatin1()};
    //"emailNotifications":"default"
    mEmailNotifications = NotificationValue{obj.value(QLatin1StringView("emailNotifications")).toString().toLatin1(),
                                            obj.value(QLatin1StringView("emailPrefOrigin")).toString().toLatin1()};
    //"unreadAlert":"nothing"
    mUnreadTrayIconAlert = obj.value(QLatin1StringView("unreadAlert")).toString();
    mMuteGroupMentions = obj.value(QLatin1StringView("muteGroupMentions")).toBool();
}

QString NotificationOptions::audioNotificationValue() const
{
    return mAudioNotificationValue;
}

void NotificationOptions::setAudioNotificationValue(const QString &audioNotificationValue)
{
    mAudioNotificationValue = audioNotificationValue;
}

QJsonObject NotificationOptions::serialize(const NotificationOptions &options)
{
    QJsonObject obj;
    obj[QLatin1StringView("audioNotificationValue")] = options.audioNotificationValue();
    obj[QLatin1StringView("disableNotifications")] = options.disableNotifications();
    obj[QLatin1StringView("desktopNotifications")] = QString::fromLatin1(options.desktopNotifications().currentValue());
    obj[QLatin1StringView("mobilePushNotifications")] = QString::fromLatin1(options.mobilePushNotification().currentValue());
    obj[QLatin1StringView("emailNotifications")] = QString::fromLatin1(options.emailNotifications().currentValue());
    obj[QLatin1StringView("unreadAlert")] = options.unreadTrayIconAlert();
    obj[QLatin1StringView("hideUnreadStatus")] = options.hideUnreadStatus();
    obj[QLatin1StringView("muteGroupMentions")] = options.muteGroupMentions();
    obj[QLatin1StringView("hideMentionStatus")] = options.hideMentionStatus();
    return obj;
}

NotificationOptions NotificationOptions::deserialize(const QJsonObject &o)
{
    qCWarning(RUQOLA_LOG) << "Not implemented yet";
    NotificationOptions options;
    options.setAudioNotificationValue(o[QLatin1StringView("audioNotificationValue")].toString());
    options.setDisableNotifications(o[QLatin1StringView("disableNotifications")].toBool());
    options.setUnreadTrayIconAlert(o[QLatin1StringView("unreadAlert")].toString());
    options.setHideUnreadStatus(o[QLatin1StringView("hideUnreadStatus")].toBool());
    options.setMuteGroupMentions(o[QLatin1StringView("muteGroupMentions")].toBool());
    options.setHideMentionStatus(o[QLatin1StringView("hideMentionStatus")].toBool());
    options.setDesktopNotifications(NotificationValue(o[QLatin1StringView("desktopNotifications")].toString().toLatin1(), QByteArray()));
    options.setMobilePushNotification(NotificationValue(o[QLatin1StringView("mobilePushNotifications")].toString().toLatin1(), QByteArray()));
    options.setEmailNotifications(NotificationValue(o[QLatin1StringView("emailNotifications")].toString().toLatin1(), QByteArray()));
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
    d << " value " << t.value;
    d << " preferenceOrigin " << t.preferenceOrigin;
    return d;
}

bool NotificationOptions::NotificationValue::operator==(const NotificationValue &other) const
{
    return other.preferenceOrigin == preferenceOrigin && other.value == value;
}

QByteArray NotificationOptions::NotificationValue::currentValue() const
{
    if (preferenceOrigin == QByteArrayLiteral("subscription") && !value.isEmpty()) {
        return value;
    }
    // Keep compatibility
    if (preferenceOrigin.isEmpty() && !value.isEmpty()) {
        return value;
    }
    if (preferenceOrigin.isEmpty() && value.isEmpty()) {
        return QByteArray();
    }
    return QByteArrayLiteral("default");
}

bool NotificationOptions::NotificationValue::isEmpty() const
{
    return preferenceOrigin.isEmpty() && value.isEmpty();
}

#include "moc_notificationoptions.cpp"
