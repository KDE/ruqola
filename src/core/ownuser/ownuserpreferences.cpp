/*
   SPDX-FileCopyrightText: 2020-2026 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "ownuserpreferences.h"

#include "ruqola_debug.h"

#include <QJsonArray>
#include <QJsonObject>

using namespace Qt::Literals::StringLiterals;
OwnUserPreferences::OwnUserPreferences() = default;

OwnUserPreferences::~OwnUserPreferences() = default;

void OwnUserPreferences::parsePreferences(const QJsonObject &replyObject)
{
    // qDebug() << " replyObject " << replyObject;
    const QJsonArray highlightsArray = replyObject.value("highlights"_L1).toArray();
    QStringList lstHighlightsWord;
    const int highlightsWordArrayCount = highlightsArray.count();
    lstHighlightsWord.reserve(highlightsWordArrayCount);
    for (int i = 0; i < highlightsWordArrayCount; ++i) {
        lstHighlightsWord << highlightsArray.at(i).toString();
    }
    setHighlightWords(lstHighlightsWord);
    setEmailNotificationMode(replyObject.value("emailNotificationMode"_L1).toString());
    setDesktopNotifications(replyObject.value("desktopNotifications"_L1).toString());
    setPushNotifications(replyObject.value("pushNotifications"_L1).toString());
    setConvertAsciiEmoji(replyObject.value("convertAsciiEmoji"_L1).toBool(true));
    setUseEmojis(replyObject.value("useEmojis"_L1).toBool(true));
    setHideRoles(replyObject.value("hideRoles"_L1).toBool(false));
    setDisplayAvatars(replyObject.value("displayAvatars"_L1).toBool(true));
    setIdleTimeLimit(replyObject.value("idleTimeLimit"_L1).toInt(-1));
    setEnableAutoAway(replyObject.value("enableAutoAway"_L1).toBool(false));
    setShowUnread(replyObject.value("sidebarShowUnread"_L1).toBool(false));
    const QString sidebarSortBy = replyObject.value("sidebarSortby"_L1).toString();
    if (sidebarSortBy == "activity"_L1) {
        setRoomListSortOrder(OwnUserPreferences::RoomListSortOrder::ByLastMessage);
    } else if (sidebarSortBy == "alphabetical"_L1) {
        setRoomListSortOrder(OwnUserPreferences::RoomListSortOrder::Alphabetically);
    }
    const QString sidebarViewMode = replyObject.value("sidebarViewMode"_L1).toString();
    if (sidebarViewMode == "medium"_L1) {
        setRoomListDisplay(OwnUserPreferences::RoomListDisplay::Medium);
    } else if (sidebarViewMode == "condensed"_L1) {
        setRoomListDisplay(OwnUserPreferences::RoomListDisplay::Condensed);
    } else if (sidebarViewMode == "extended"_L1) {
        setRoomListDisplay(OwnUserPreferences::RoomListDisplay::Extended);
    } else {
        qCWarning(RUQOLA_LOG) << "RoomListDisplay is not defined ?  " << sidebarViewMode;
    }
    setShowRoomAvatar(replyObject.value("sidebarDisplayAvatar"_L1).toBool(false));
    setShowFavorite(replyObject.value("sidebarShowFavorites"_L1).toBool(false));
    setReceiveLoginDetectionEmail(replyObject.value("receiveLoginDetectionEmail"_L1).toBool(true));
    setNewMessageNotification(replyObject.value("newMessageNotification"_L1).toString().toLatin1());
    setNewRoomNotification(replyObject.value("newRoomNotification"_L1).toString().toLatin1());
    setNotificationsSoundVolume(replyObject.value("notificationsSoundVolume"_L1).toInt());
    setMuteFocusedConversations(replyObject.value("muteFocusedConversations"_L1).toBool(false));

    // featuresPreview
}

bool OwnUserPreferences::operator==(const OwnUserPreferences &other) const
{
    return mHighlightWords == other.highlightWords() && mEmailNotificationMode == other.emailNotificationMode()
        && mDesktopNotifications == other.desktopNotifications() && mUseEmojis == other.useEmojis() && mConvertAsciiEmoji == other.convertAsciiEmoji()
        && mHideRoles == other.hideRoles() && mDisplayAvatars == other.displayAvatars() && mIdleTimeLimit == other.idleTimeLimit()
        && mEnableAutoAway == other.enableAutoAway() && mPushNotifications == other.pushNotifications() && mShowUnread == other.showUnread()
        && mShowRoomAvatar == other.showRoomAvatar() && mShowFavorite == other.showFavorite() && mRoomListSortOrder == other.roomListSortOrder()
        && mReceiveLoginDetectionEmail == other.receiveLoginDetectionEmail() && mRoomListDisplay == other.roomListDisplay()
        && mNewMessageNotification == other.newMessageNotification() && mNewRoomNotification == other.newRoomNotification()
        && mNotificationsSoundVolume == other.notificationsSoundVolume() && mMuteFocusedConversations == other.muteFocusedConversations();
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

OwnUserPreferences::RoomListDisplay OwnUserPreferences::roomListDisplay() const
{
    return mRoomListDisplay;
}

void OwnUserPreferences::setRoomListDisplay(RoomListDisplay newRoomListDisplay)
{
    mRoomListDisplay = newRoomListDisplay;
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

QByteArray OwnUserPreferences::newMessageNotification() const
{
    return mNewMessageNotification;
}

void OwnUserPreferences::setNewMessageNotification(const QByteArray &newNewMessageNotification)
{
    mNewMessageNotification = newNewMessageNotification;
}

QByteArray OwnUserPreferences::newRoomNotification() const
{
    return mNewRoomNotification;
}

void OwnUserPreferences::setNewRoomNotification(const QByteArray &newNewRoomNotification)
{
    mNewRoomNotification = newNewRoomNotification;
}

int OwnUserPreferences::notificationsSoundVolume() const
{
    return mNotificationsSoundVolume;
}

void OwnUserPreferences::setNotificationsSoundVolume(int newNotificationsSoundVolume)
{
    mNotificationsSoundVolume = newNotificationsSoundVolume;
}

bool OwnUserPreferences::muteFocusedConversations() const
{
    return mMuteFocusedConversations;
}

void OwnUserPreferences::setMuteFocusedConversations(bool newMuteFocusedConversations)
{
    mMuteFocusedConversations = newMuteFocusedConversations;
}

QDebug operator<<(QDebug d, const OwnUserPreferences &t)
{
    d.space() << "mHighlightWords" << t.highlightWords();
    d.space() << "mEmailNotificationMode" << t.emailNotificationMode();
    d.space() << "mDesktopNotifications" << t.desktopNotifications();
    d.space() << "mUseEmojis" << t.useEmojis();
    d.space() << "mConvertAsciiEmoji" << t.convertAsciiEmoji();
    d.space() << "mHideRoles" << t.hideRoles();
    d.space() << "mDisplayAvatars" << t.displayAvatars();
    d.space() << "mIdleTimeLimit" << t.idleTimeLimit();
    d.space() << "mEnableAutoAway" << t.enableAutoAway();
    d.space() << "mPushNotifications" << t.pushNotifications();
    d.space() << "mShowUnread" << t.showUnread();
    d.space() << "mRoomListSortOrder" << t.roomListSortOrder();
    d.space() << "mShowRoomAvatar" << t.showRoomAvatar();
    d.space() << "mShowFavorite" << t.showFavorite();
    d.space() << "mReceiveLoginDetectionEmail" << t.receiveLoginDetectionEmail();
    d.space() << "mRoomListDisplay" << t.roomListDisplay();
    d.space() << "newMessageNotification" << t.newMessageNotification();
    d.space() << "newRoomNotification" << t.newRoomNotification();
    d.space() << "notificationsSoundVolume" << t.notificationsSoundVolume();
    d.space() << "muteFocusedConversations" << t.muteFocusedConversations();
    d.space() << "featurePreviewPreferences" << t.featurePreviewPreferences();
    return d;
}

bool OwnUserPreferences::hasFeature(FeaturePreviewPreferences::FeaturePreviewType type) const
{
    return mFeaturePreviewPreferences.hasFeature(type);
}

bool OwnUserPreferences::hasFeaturePreview() const
{
    return mFeaturePreviewPreferences.hasFeaturePreview();
}

FeaturePreviewPreferences OwnUserPreferences::featurePreviewPreferences() const
{
    return mFeaturePreviewPreferences;
}

#include "moc_ownuserpreferences.cpp"
