/*
   SPDX-FileCopyrightText: 2020-2024 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "ownuserpreferences.h"

#include <QJsonArray>
#include <QJsonObject>

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
    setDisplayAvatars(replyObject.value(QLatin1String("displayAvatars")).toBool(true));
    setIdleTimeLimit(replyObject.value(QLatin1String("idleTimeLimit")).toInt(-1));
    setEnableAutoAway(replyObject.value(QLatin1String("enableAutoAway")).toBool(false));
    setShowUnread(replyObject.value(QLatin1String("sidebarShowUnread")).toBool(false));
    const QString sidebarSortBy = replyObject.value(QLatin1String("sidebarSortby")).toString();
    if (sidebarSortBy == QLatin1String("activity")) {
        setRoomListSortOrder(OwnUserPreferences::RoomListSortOrder::ByLastMessage);
    } else if (sidebarSortBy == QLatin1String("alphabetical")) {
        setRoomListSortOrder(OwnUserPreferences::RoomListSortOrder::Alphabetically);
    }
    setShowRoomAvatar(replyObject.value(QLatin1String("sidebarDisplayAvatar")).toBool(false));
    setShowFavorite(replyObject.value(QLatin1String("sidebarShowFavorites")).toBool(false));
    setReceiveLoginDetectionEmail(replyObject.value(QLatin1String("receiveLoginDetectionEmail")).toBool(true));

    qDebug() << " replyObject " << replyObject;
}

bool OwnUserPreferences::operator==(const OwnUserPreferences &other) const
{
    return mHighlightWords == other.highlightWords() && mEmailNotificationMode == other.emailNotificationMode()
        && mDesktopNotifications == other.desktopNotifications() && mUseEmojis == other.useEmojis() && mConvertAsciiEmoji == other.convertAsciiEmoji()
        && mHideRoles == other.hideRoles() && mDisplayAvatars == other.displayAvatars() && mIdleTimeLimit == other.idleTimeLimit()
        && mEnableAutoAway == other.enableAutoAway() && mPushNotifications == other.pushNotifications() && mShowUnread == other.showUnread()
        && mShowRoomAvatar == other.showRoomAvatar() && mShowFavorite == other.showFavorite() && mRoomListSortOrder == other.roomListSortOrder()
        && mReceiveLoginDetectionEmail == other.receiveLoginDetectionEmail();
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

bool OwnUserPreferences::showUnread() const
{
    return mShowUnread;
}

void OwnUserPreferences::setShowUnread(bool newShowUnread)
{
    mShowUnread = newShowUnread;
}

OwnUserPreferences::RoomListSortOrder OwnUserPreferences::roomListSortOrder() const
{
    return mRoomListSortOrder;
}

void OwnUserPreferences::setRoomListSortOrder(OwnUserPreferences::RoomListSortOrder roomListSortOrder)
{
    mRoomListSortOrder = roomListSortOrder;
}

bool OwnUserPreferences::showRoomAvatar() const
{
    return mShowRoomAvatar;
}

void OwnUserPreferences::setShowRoomAvatar(bool newShowRoomAvatar)
{
    mShowRoomAvatar = newShowRoomAvatar;
}

bool OwnUserPreferences::showFavorite() const
{
    return mShowFavorite;
}

void OwnUserPreferences::setShowFavorite(bool newShowFavorite)
{
    mShowFavorite = newShowFavorite;
}

bool OwnUserPreferences::receiveLoginDetectionEmail() const
{
    return mReceiveLoginDetectionEmail;
}

void OwnUserPreferences::setReceiveLoginDetectionEmail(bool newReceiveLoginDetectionEmail)
{
    mReceiveLoginDetectionEmail = newReceiveLoginDetectionEmail;
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

bool OwnUserPreferences::displayAvatars() const
{
    return mDisplayAvatars;
}

void OwnUserPreferences::setDisplayAvatars(bool hideAvatars)
{
    mDisplayAvatars = hideAvatars;
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
    d << "mDisplayAvatars " << t.displayAvatars();
    d << "mIdleTimeLimit " << t.idleTimeLimit();
    d << "mEnableAutoAway " << t.enableAutoAway();
    d << "mPushNotifications " << t.pushNotifications();
    d << "mShowUnread " << t.showUnread();
    d << "mRoomListSortOrder " << t.roomListSortOrder();
    d << "mShowRoomAvatar " << t.showRoomAvatar();
    d << "mShowFavorite " << t.showFavorite();
    d << "mReceiveLoginDetectionEmail " << t.receiveLoginDetectionEmail();
    return d;
}

#include "moc_ownuserpreferences.cpp"
