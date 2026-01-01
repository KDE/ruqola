/*
   SPDX-FileCopyrightText: 2017-2026 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "user.h"
QT_IMPL_METATYPE_EXTERN_TAGGED(User, Ruqola_User)

#include "ruqola_debug.h"
#include "ruqola_specialwarning_debug.h"
#include "utils.h"

#include <KLocalizedString>

#include <QJsonArray>
#include <QJsonObject>
#include <QTimeZone>

using namespace Qt::Literals::StringLiterals;
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

QByteArray User::userId() const
{
    return mUserId;
}

void User::setUserId(const QByteArray &userId)
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
        && (mBio == other.bio()) && (mNickName == other.nickName()) && (mType == other.type());
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
    d.space() << "email " << t.email;
    d.space() << "verified " << t.verified;
    return d;
}

QDebug operator<<(QDebug d, const User &t)
{
    d.space() << "Name " << t.name();
    d.space() << "UserId " << t.userId();
    d.space() << "Status " << t.status();
    d.space() << "UserName " << t.userName();
    d.space() << "UtcOffset " << t.utcOffset();
    d.space() << "StatusText " << t.statusText();
    d.space() << "roles " << t.roles();
    d.space() << "CreatedAt " << t.createdAt();
    d.space() << "Last Login " << t.lastLogin();
    d.space() << "userEmailsInfo " << t.userEmailsInfo();
    d.space() << "active " << t.active();
    d.space() << "mRequirePasswordChange " << t.requirePasswordChange();
    d.space() << "mBio " << t.bio();
    d.space() << "mNickName " << t.nickName();
    d.space() << "mType " << t.type();
    return d;
}

// FIXME Add autotest for it!
void User::parseUserRestApi(const QJsonObject &object, const QList<RoleInfo> &roleInfo)
{
    setUserId(object.value("_id"_L1).toString().toLatin1());
    setName(object.value("name"_L1).toString());
    setStatus(Utils::presenceStatusFromString(object.value("status"_L1).toString()));
    setUserName(object.value("username"_L1).toString());
    setStatusText(object.value("statusText"_L1).toString());
    setUtcOffset(object.value("utcOffset"_L1).toDouble());
    setActive(object.value("active"_L1).toBool(true)); // By default it's active
    setBio(object.value("bio"_L1).toString());
    setNickName(object.value("nickname"_L1).toString());
    setType(object.value("type"_L1).toString());
    const QJsonArray rolesArray = object.value("roles"_L1).toArray();
    QStringList roles;
    const int total = rolesArray.size();
    roles.reserve(total);
    for (int i = 0; i < total; ++i) {
        roles.append(rolesArray.at(i).toString());
    }
    setRoles(roles, roleInfo);
    if (object.contains("createdAt"_L1)) {
        setCreatedAt(QDateTime::fromMSecsSinceEpoch(Utils::parseIsoDate(u"createdAt"_s, object), QTimeZone::UTC));
    }
    if (object.contains("lastLogin"_L1)) {
        setLastLogin(QDateTime::fromMSecsSinceEpoch(Utils::parseIsoDate(u"lastLogin"_s, object), QTimeZone::UTC));
    }
    if (object.contains("emails"_L1)) {
        const QJsonArray emails = object.value("emails"_L1).toArray();
        if (emails.count() > 1) {
            qCWarning(RUQOLA_LOG) << " Users info has more that 1 emails. Bug or missing feature" << emails;
        } else {
            const QJsonObject emailObj = emails.at(0).toObject();
            const UserEmailsInfo info{
                .email = emailObj.value("address"_L1).toString(),
                .verified = emailObj.value("verified"_L1).toBool(),
            };
            setUserEmailsInfo(std::move(info));
        }
    }
    setRequirePasswordChange(object.value("requirePasswordChange"_L1).toBool(false));

    // TODO emails
    // qDebug() << " object "  << object;
}

void User::parseUser(const QVariantList &list)
{
    if (list.count() != 4) {
        qCDebug(RUQOLA_SPECIALWARNING_LOG) << " List argument different of 4! It's a bug: " << list;
    }
    setUserId(list.at(0).toString().toLatin1());
    setUserName(list.at(1).toString());
    const int valueStatus = list.at(2).toInt();
    if (valueStatus == 0) {
        setStatus(PresenceStatus::Offline);
    } else if (valueStatus == 1) {
        setStatus(PresenceStatus::Online);
    } else if (valueStatus == 2) {
        setStatus(PresenceStatus::Away);
    } else if (valueStatus == 3) {
        setStatus(PresenceStatus::Busy);
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
    if (roleStr == "user"_L1) {
        ri18n = i18n("User");
    } else if (roleStr == "admin"_L1) {
        ri18n = i18n("Administrator");
    } else if (roleStr == "bot"_L1) {
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
    o["identifier"_L1] = QString::fromLatin1(user.userId());
    o["name"_L1] = user.name();
    o["username"_L1] = user.userName();
    o["statusText"_L1] = user.statusText();
    o["bio"_L1] = user.bio();
    o["nickname"_L1] = user.nickName();
    if (!user.type().isEmpty()) {
        o["type"_L1] = user.type();
    }
    o["active"_L1] = user.active();
    o["status"_L1] = Utils::presenceStatusToString(user.status());
    o["utcOffset"_L1] = user.utcOffset();
    return o;
}

User User::deserialize(const QJsonObject &o)
{
    User user;
    user.setUserId(o.value("identifier"_L1).toString().toLatin1());
    user.setName(o.value("name"_L1).toString());
    user.setStatus(Utils::presenceStatusFromString(o.value("status"_L1).toString()));
    user.setUserName(o.value("username"_L1).toString());
    user.setStatusText(o.value("statusText"_L1).toString());
    user.setUtcOffset(o.value("utcOffset"_L1).toDouble());
    user.setActive(o.value("active"_L1).toBool(true)); // By default it's active
    user.setBio(o.value("bio"_L1).toString());
    user.setNickName(o.value("nickname"_L1).toString());
    if (!user.type().isEmpty()) {
        user.setType(o.value("type"_L1).toString());
    }
    return user;
}

QString User::nickName() const
{
    return mNickName;
}

void User::setNickName(const QString &newNickName)
{
    mNickName = newNickName;
}

QString User::type() const
{
    return mType;
}

void User::setType(const QString &newType)
{
    mType = newType;
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
    const QJsonObject fields = object.value("fields"_L1).toObject();
    setUserId(object.value("id"_L1).toString().toLatin1());
    setName(fields.value("name"_L1).toString());
    setNickName(fields.value("nickname"_L1).toString());
    setStatus(Utils::presenceStatusFromString(fields.value("status"_L1).toString()));
    setUserName(fields.value("username"_L1).toString());
    setStatusText(fields.value("statusText"_L1).toString());
    setUtcOffset(fields.value("utcOffset"_L1).toDouble());
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
    const QJsonArray fieldsArray = object.value("items"_L1).toArray();
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
