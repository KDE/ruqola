/*
   SPDX-FileCopyrightText: 2020-2022 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "ownuserpreferences.h"

#include <QJsonArray>

OwnUserPreferences::OwnUserPreferences() = default;

OwnUserPreferences::~OwnUserPreferences() = default;

void OwnUserPreferences::parsePreferences(const QJsonObject &replyObject)
{
    // qDebug() << " replyObject " << replyObject;
    const QJsonArray highlightsArray = replyObject.value(QLatin1String("highlights")).toArray();
    QStringList lstHighlightsWord;
    const int highlightsWordArrayCount = highlightsArray.count();
    lstHighlightsWord.reserve(highlightsWordArrayCount);
    for (int i = 0; i < highlightsWordArrayCount; ++i) {
        lstHighlightsWord << highlightsArray.at(i).toString();
    }
    setHighlightWords(lstHighlightsWord);
    setEmailNotificationMode(replyObject.value(QLatin1String("emailNotificationMode")).toString());
    setDesktopNotifications(replyObject.value(QLatin1String("desktopNotifications")).toString());
    setPushNotifications(replyObject.value(QLatin1String("pushNotifications")).toString());
    setConvertAsciiEmoji(replyObject.value(QLatin1String("convertAsciiEmoji")).toBool(true));
    setUseEmojis(replyObject.value(QLatin1String("useEmojis")).toBool(true));
    setHideRoles(replyObject.value(QLatin1String("hideRoles")).toBool(false));
    setHideAvatars(replyObject.value(QLatin1String("hideAvatars")).toBool(false));
    setIdleTimeLimit(replyObject.value(QLatin1String("idleTimeLimit")).toInt(-1));
    setEnableAutoAway(replyObject.value(QLatin1String("enableAutoAway")).toBool(false));
    setMessageViewMode(replyObject.value(QLatin1String("messageViewMode")).toInt(-1));
}

bool OwnUserPreferences::operator==(const OwnUserPreferences &other) const
{
    return mHighlightWords == other.highlightWords() && mEmailNotificationMode == other.emailNotificationMode()
        && mDesktopNotifications == other.desktopNotifications() && mUseEmojis == other.useEmojis() && mConvertAsciiEmoji == other.convertAsciiEmoji()
        && mHideRoles == other.hideRoles() && mHideAvatars == other.hideAvatars() && mIdleTimeLimit == other.idleTimeLimit()
        && mEnableAutoAway == other.enableAutoAway() && mPushNotifications == other.pushNotifications() && mMessageViewMode == other.messageViewMode();
}

QStringList OwnUserPreferences::highlightWords() const
{
    return mHighlightWords;
}

void OwnUserPreferences::updateHighlightWords(const QJsonArray &highlightsArray)
{
    QStringList lstHighlightsWord;
    const int highlightsWordArrayCount = highlightsArray.count();
    lstHighlightsWord.reserve(highlightsWordArrayCount);
    for (int i = 0; i < highlightsWordArrayCount; ++i) {
        lstHighlightsWord << highlightsArray.at(i).toString();
    }
    setHighlightWords(lstHighlightsWord);
}

void OwnUserPreferences::setHighlightWords(const QStringList &highlightWords)
{
    mHighlightWords = highlightWords;
}

QString OwnUserPreferences::emailNotificationMode() const
{
    return mEmailNotificationMode;
}

void OwnUserPreferences::setEmailNotificationMode(const QString &emailNotificationMode)
{
    mEmailNotificationMode = emailNotificationMode;
}

QString OwnUserPreferences::desktopNotifications() const
{
    return mDesktopNotifications;
}

void OwnUserPreferences::setDesktopNotifications(const QString &desktopNotifications)
{
    mDesktopNotifications = desktopNotifications;
}

void OwnUserPreferences::setPushNotifications(const QString &pushNotifications)
{
    mPushNotifications = pushNotifications;
}

QString OwnUserPreferences::pushNotifications() const
{
    return mPushNotifications;
}

int OwnUserPreferences::messageViewMode() const
{
    return mMessageViewMode;
}

void OwnUserPreferences::setMessageViewMode(int newMessageViewMode)
{
    mMessageViewMode = newMessageViewMode;
}

bool OwnUserPreferences::convertAsciiEmoji() const
{
    return mConvertAsciiEmoji;
}

void OwnUserPreferences::setConvertAsciiEmoji(bool convertAsciiEmoji)
{
    mConvertAsciiEmoji = convertAsciiEmoji;
}

bool OwnUserPreferences::useEmojis() const
{
    return mUseEmojis;
}

void OwnUserPreferences::setUseEmojis(bool useEmojis)
{
    mUseEmojis = useEmojis;
}

bool OwnUserPreferences::hideRoles() const
{
    return mHideRoles;
}

void OwnUserPreferences::setHideRoles(bool hideRoles)
{
    mHideRoles = hideRoles;
}

bool OwnUserPreferences::hideAvatars() const
{
    return mHideAvatars;
}

void OwnUserPreferences::setHideAvatars(bool hideAvatars)
{
    mHideAvatars = hideAvatars;
}

int OwnUserPreferences::idleTimeLimit() const
{
    return mIdleTimeLimit;
}

void OwnUserPreferences::setIdleTimeLimit(int newIdleTimeLimit)
{
    mIdleTimeLimit = newIdleTimeLimit;
}

bool OwnUserPreferences::enableAutoAway() const
{
    return mEnableAutoAway;
}

void OwnUserPreferences::setEnableAutoAway(bool newEnableAutoAway)
{
    mEnableAutoAway = newEnableAutoAway;
}

QDebug operator<<(QDebug d, const OwnUserPreferences &t)
{
    d << "mHighlightWords " << t.highlightWords();
    d << "mEmailNotificationMode " << t.emailNotificationMode();
    d << "mDesktopNotifications " << t.desktopNotifications();
    d << "mUseEmojis " << t.useEmojis();
    d << "mConvertAsciiEmoji " << t.convertAsciiEmoji();
    d << "mHideRoles " << t.hideRoles();
    d << "mHideAvatars " << t.hideAvatars();
    d << "mIdleTimeLimit " << t.idleTimeLimit();
    d << "mEnableAutoAway " << t.enableAutoAway();
    d << "mPushNotifications " << t.pushNotifications();
    d << "mMessageViewMode " << t.messageViewMode();
    return d;
}
