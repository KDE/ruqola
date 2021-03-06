/*
   Copyright (c) 2020-2021 Laurent Montel <montel@kde.org>

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

#include "ownuserpreferences.h"

#include <QJsonArray>

OwnUserPreferences::OwnUserPreferences()
{
}

OwnUserPreferences::~OwnUserPreferences() = default;

void OwnUserPreferences::parsePreferences(const QJsonObject &replyObject)
{
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
    setMobileNotifications(replyObject.value(QLatin1String("mobileNotifications")).toString());
    setConvertAsciiEmoji(replyObject.value(QLatin1String("convertAsciiEmoji")).toBool(true));
    setUseEmojis(replyObject.value(QLatin1String("useEmojis")).toBool(true));
    setHideRoles(replyObject.value(QLatin1String("hideRoles")).toBool(false));
    setHideAvatars(replyObject.value(QLatin1String("hideAvatars")).toBool(false));
}

bool OwnUserPreferences::operator==(const OwnUserPreferences &other) const
{
    return mHighlightWords == other.highlightWords() && mEmailNotificationMode == other.emailNotificationMode()
        && mDesktopNotifications == other.desktopNotifications() && mMobileNotifications == other.mobileNotifications() && mUseEmojis == other.useEmojis()
        && mConvertAsciiEmoji == other.convertAsciiEmoji() && mHideRoles == other.hideRoles() && mHideAvatars == other.hideAvatars();
}

QStringList OwnUserPreferences::highlightWords() const
{
    return mHighlightWords;
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

QString OwnUserPreferences::mobileNotifications() const
{
    return mMobileNotifications;
}

void OwnUserPreferences::setMobileNotifications(const QString &mobileNotifications)
{
    mMobileNotifications = mobileNotifications;
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

QDebug operator<<(QDebug d, const OwnUserPreferences &t)
{
    d << "mHighlightWords " << t.highlightWords();
    d << "mEmailNotificationMode " << t.emailNotificationMode();
    d << "mDesktopNotifications " << t.desktopNotifications();
    d << "mMobileNotifications " << t.mobileNotifications();
    d << "mUseEmojis " << t.useEmojis();
    d << "mConvertAsciiEmoji " << t.convertAsciiEmoji();
    d << "mHideRoles " << t.hideRoles();
    d << "mHideAvatars " << t.hideAvatars();
    return d;
}
