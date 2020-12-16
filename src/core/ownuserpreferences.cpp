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
}

bool OwnUserPreferences::operator ==(const OwnUserPreferences &other) const
{
    return mHighlightWords == other.highlightWords()
            && mEmailNotificationMode == other.emailNotificationMode()
            && mDesktopNotifications == other.desktopNotifications()
            && mMobileNotifications == other.mobileNotifications();
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

QDebug operator <<(QDebug d, const OwnUserPreferences &t)
{
    d << "mHighlightWords " << t.highlightWords();
    d << "mEmailNotificationMode " << t.emailNotificationMode();
    d << "mDesktopNotifications " << t.desktopNotifications();
    d << "mMobileNotifications " << t.mobileNotifications();
    return d;
}
