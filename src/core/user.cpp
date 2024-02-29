/*
   SPDX-FileCopyrightText: 2017-2024 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "user.h"
#include "ruqola_debug.h"
#include "ruqola_specialwarning_debug.h"
#include "utils.h"

#include <KLocalizedString>

#include <QJsonArray>
#include <QJsonObject>

User::User() = default;

User::~User() = default;

QString User::name() const
{
    return mName;
}

void User::setName(const QString &name)
{
    mName = name;
}

QString User::userId() const
{
    return mUserId;
}

void User::setUserId(const QString &userId)
{
    mUserId = userId;
}

User::PresenceStatus User::status() const
{
    return mStatus;
}

void User::setStatus(PresenceStatus s)
{
    mStatus = s;
}

bool User::operator==(const User &other) const
{
    return (mName == other.name()) && (mUserId == other.userId()) && (mStatus == other.status()) && (mUserName == other.userName())
        && (mUtcOffset == other.utcOffset()) && (mStatusText == other.statusText()) && (mRoles == other.roles()) && (mCreatedAt == other.createdAt())
        && (mLastLogin == other.lastLogin()) && (mActive == other.active()) && (mRequirePasswordChange == other.requirePasswordChange())
        && (mBio == other.bio()) && (mNickName == other.nickName());
}

bool User::operator!=(const User &other) const
{
    return !operator==(other);
}

QString User::userName() const
{
    return mUserName;
}

void User::setUserName(const QString &userName)
{
    mUserName = userName;
}

bool User::isValid() const
{
    return !mUserName.isEmpty() || !mName.isEmpty();
}

double User::utcOffset() const
{
    return mUtcOffset;
}

void User::setUtcOffset(double utcOffset)
{
    mUtcOffset = utcOffset;
}

QString User::statusText() const
{
    return mStatusText;
}

void User::setStatusText(const QString &statusText)
{
    mStatusText = statusText;
}

QDebug operator<<(QDebug d, const User::UserEmailsInfo &t)
{
    d << "email " << t.email;
    d << "verified " << t.verified;
    return d;
}

QDebug operator<<(QDebug d, const User &t)
{
    d << " Name " << t.name();
    d << " UserId " << t.userId();
    d << " Status " << t.status();
    d << " UserName " << t.userName();
    d << " UtcOffset " << t.utcOffset();
    d << " StatusText " << t.statusText();
    d << " roles " << t.roles();
    d << " CreatedAt " << t.createdAt();
    d << " Last Login " << t.lastLogin();
    d << " userEmailsInfo " << t.userEmailsInfo();
    d << " active " << t.active();
    d << " mRequirePasswordChange " << t.requirePasswordChange();
    d << " mBio " << t.bio();
    d << " mNickName " << t.nickName();
    return d;
}

// FIXME Add autotest for it!
void User::parseUserRestApi(const QJsonObject &object, const QList<RoleInfo> &roleInfo)
{
    setUserId(object.value(QLatin1String("_id")).toString());
    setName(object.value(QLatin1String("name")).toString());
    setStatus(Utils::presenceStatusFromString(object.value(QLatin1String("status")).toString()));
    setUserName(object.value(QLatin1String("username")).toString());
    setStatusText(object.value(QLatin1String("statusText")).toString());
    setUtcOffset(object.value(QLatin1String("utcOffset")).toDouble());
    setActive(object.value(QLatin1String("active")).toBool(true)); // By default it's active
    setBio(object.value(QLatin1String("bio")).toString());
    setNickName(object.value(QLatin1String("nickname")).toString());
    const QJsonArray rolesArray = object.value(QStringLiteral("roles")).toArray();
    QStringList roles;
    const int total = rolesArray.size();
    roles.reserve(total);
    for (int i = 0; i < total; ++i) {
        roles.append(rolesArray.at(i).toString());
    }
    setRoles(roles, roleInfo);
    if (object.contains(QLatin1String("createdAt"))) {
        setCreatedAt(QDateTime::fromMSecsSinceEpoch(Utils::parseIsoDate(QStringLiteral("createdAt"), object)));
    }
    if (object.contains(QLatin1String("lastLogin"))) {
        setLastLogin(QDateTime::fromMSecsSinceEpoch(Utils::parseIsoDate(QStringLiteral("lastLogin"), object)));
    }
    if (object.contains(QLatin1String("emails"))) {
        const QJsonArray emails = object.value(QStringLiteral("emails")).toArray();
        if (emails.count() > 1) {
            qCWarning(RUQOLA_LOG) << " Users info has more that 1 emails. Bug or missing feature" << emails;
        } else {
            const QJsonObject emailObj = emails.at(0).toObject();
            UserEmailsInfo info;
            info.email = emailObj.value(QLatin1String("address")).toString();
            info.verified = emailObj.value(QLatin1String("verified")).toBool();
            setUserEmailsInfo(info);
        }
    }
    setRequirePasswordChange(object.value(QLatin1String("requirePasswordChange")).toBool(false));

    // TODO emails
    // qDebug() << " object "  << object;
}

void User::parseUser(const QVariantList &list)
{
    if (list.count() != 4) {
        qCDebug(RUQOLA_SPECIALWARNING_LOG) << " List argument different of 4! It's a bug: " << list;
    }
    setUserId(list.at(0).toString());
    setUserName(list.at(1).toString());
    const int valueStatus = list.at(2).toInt();
    if (valueStatus == 0) {
        setStatus(PresenceStatus::PresenceOffline);
    } else if (valueStatus == 1) {
        setStatus(PresenceStatus::PresenceOnline);
    } else if (valueStatus == 2) {
        setStatus(PresenceStatus::PresenceAway);
    } else if (valueStatus == 3) {
        setStatus(PresenceStatus::PresenceBusy);
    } else {
        qCWarning(RUQOLA_LOG) << " Invalid status value" << valueStatus;
        return;
    }
    const QVariant customText = list.at(3);
    if (customText.isValid()) {
        setStatusText(customText.toString());
    }
}

QStringList User::roles() const
{
    return mRoles;
}

QString User::roleI18n(const QString &roleStr, const QList<RoleInfo> &roleInfo)
{
    QString ri18n;
    if (roleStr == QLatin1String("user")) {
        ri18n = i18n("User");
    } else if (roleStr == QLatin1String("admin")) {
        ri18n = i18n("Administrator");
    } else if (roleStr == QLatin1String("bot")) {
        ri18n = i18n("Bot");
    } else {
        for (const RoleInfo &info : roleInfo) {
            if (roleStr == info.identifier()) {
                ri18n = info.name();
                break;
            }
        }
        if (ri18n.isEmpty()) {
            ri18n = roleStr;
        }
    }
    return ri18n;
}

QString User::bio() const
{
    return mBio;
}

void User::setBio(const QString &newBio)
{
    mBio = newBio;
}

QJsonObject User::serialize(const User &user)
{
    QJsonObject o;
    o[QLatin1String("identifier")] = user.userId();
    o[QLatin1String("name")] = user.name();
    o[QLatin1String("username")] = user.userName();
    o[QLatin1String("statusText")] = user.statusText();
    o[QLatin1String("bio")] = user.bio();
    o[QLatin1String("nickname")] = user.nickName();

    // Add status/utcoffset/active

    // TODO
    return o;
}

User User::deserialize(const QJsonObject &o)
{
    User user;
    user.setUserId(o.value(QLatin1String("identifier")).toString());
    user.setName(o.value(QLatin1String("name")).toString());
    user.setStatus(Utils::presenceStatusFromString(o.value(QLatin1String("status")).toString()));
    user.setUserName(o.value(QLatin1String("username")).toString());
    user.setStatusText(o.value(QLatin1String("statusText")).toString());
    user.setUtcOffset(o.value(QLatin1String("utcOffset")).toDouble());
    user.setActive(o.value(QLatin1String("active")).toBool(true)); // By default it's active
    user.setBio(o.value(QLatin1String("bio")).toString());
    user.setNickName(o.value(QLatin1String("nickname")).toString());
    // TODO
    return {};
}

QString User::nickName() const
{
    return mNickName;
}

void User::setNickName(const QString &newNickName)
{
    mNickName = newNickName;
}

void User::setRoles(const QStringList &roles, const QList<RoleInfo> &roleInfo)
{
    QStringList rolesI18n;
    rolesI18n.reserve(roles.count());
    for (const QString &role : roles) {
        rolesI18n.append(User::roleI18n(role, roleInfo));
    }
    mI18nRoles = rolesI18n;
    mRoles = roles;
}

QDateTime User::createdAt() const
{
    return mCreatedAt;
}

void User::setCreatedAt(const QDateTime &createdAt)
{
    mCreatedAt = createdAt;
}

QDateTime User::lastLogin() const
{
    return mLastLogin;
}

void User::setLastLogin(const QDateTime &lastLogin)
{
    mLastLogin = lastLogin;
}

User::UserEmailsInfo User::userEmailsInfo() const
{
    return mUserEmailsInfo;
}

void User::setUserEmailsInfo(const UserEmailsInfo &userEmailsInfo)
{
    mUserEmailsInfo = userEmailsInfo;
}

// TODO remove it ????
QStringList User::i18nRoles() const
{
    return mI18nRoles;
}

bool User::active() const
{
    return mActive;
}

void User::setActive(bool newActive)
{
    mActive = newActive;
}

void User::parseUser(const QJsonObject &object)
{
    const QJsonObject fields = object.value(QLatin1String("fields")).toObject();
    setUserId(object.value(QLatin1String("id")).toString());
    setName(fields.value(QLatin1String("name")).toString());
    setNickName(fields.value(QLatin1String("nickname")).toString());
    setStatus(Utils::presenceStatusFromString(fields.value(QLatin1String("status")).toString()));
    setUserName(fields.value(QLatin1String("username")).toString());
    setStatusText(fields.value(QLatin1String("statusText")).toString());
    setUtcOffset(fields.value(QLatin1String("utcOffset")).toDouble());
    // TODO active ?
}

QString User::generateStatusStr() const
{
    return Utils::displaytextFromPresenceStatus(mStatus);
}

bool User::requirePasswordChange() const
{
    return mRequirePasswordChange;
}

void User::setRequirePasswordChange(bool newRequirePasswordChange)
{
    mRequirePasswordChange = newRequirePasswordChange;
}

QString User::iconFromStatus() const
{
    return Utils::iconFromPresenceStatus(mStatus);
}

QList<User> User::parseUsersList(const QJsonObject &object, const QList<RoleInfo> &roleInfo)
{
    const QJsonArray fieldsArray = object.value(QLatin1String("items")).toArray();
    QList<User> users;
    for (const QJsonValue &current : fieldsArray) {
        if (current.type() == QJsonValue::Object) {
            const QJsonObject userObject = current.toObject();
            User user;
            user.parseUserRestApi(userObject, roleInfo);
            users.append(std::move(user));
        } else {
            qCWarning(RUQOLA_LOG) << "Problem when parsing users" << current;
        }
    }
    return users;
}

#include "moc_user.cpp"
