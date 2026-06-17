/*
   SPDX-FileCopyrightText: 2020-2026 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "bannedusers.h"

#include "ruqola_commands_debug.h"
#include <QJsonArray>
#include <QJsonObject>

QT_IMPL_METATYPE_EXTERN_TAGGED(BannedUsers, Ruqola_BannedUsers)
using namespace Qt::Literals::StringLiterals;
BannedUsers::BannedUsers() = default;

bool BannedUsers::isEmpty() const
{
    return mBannedUsers.isEmpty();
}

void BannedUsers::clear()
{
    mBannedUsers.clear();
}

int BannedUsers::count() const
{
    return mBannedUsers.count();
}

BannedUser BannedUsers::at(int index) const
{
    if (index < 0 || index >= mBannedUsers.count()) {
        qCWarning(RUQOLA_COMMANDS_LOG) << "Invalid index " << index;
        return {};
    }
    return mBannedUsers.at(index);
}

void BannedUsers::parseMoreBannedUsers(const QJsonObject &bannerObj)
{
    const int commandsCount = bannerObj["count"_L1].toInt();
    mOffset = bannerObj["offset"_L1].toInt();
    mTotal = bannerObj["total"_L1].toInt();
    parseListBannedUsers(bannerObj);
    mBannedUsersCount += commandsCount;
}

void BannedUsers::parseListBannedUsers(const QJsonObject &commandsObj)
{
    const QJsonArray bannedUsesArray = commandsObj["bannedUsers"_L1].toArray();
    mBannedUsers.reserve(mBannedUsers.count() + bannedUsesArray.count());
    for (const auto &current : bannedUsesArray) {
        if (current.type() == QJsonValue::Object) {
            const QJsonObject bannedUsersObj = current.toObject();
            BannedUser m;
            m.parseBannedUser(bannedUsersObj);
            mBannedUsers.append(std::move(m));
        } else {
            qCWarning(RUQOLA_COMMANDS_LOG) << "Problem when parsing bannedusers" << current.type();
        }
    }
}

int BannedUsers::bannedUsersCount() const
{
    return mBannedUsersCount;
}

void BannedUsers::setBannedUsersCount(int commandsCount)
{
    mBannedUsersCount = commandsCount;
}

QList<BannedUser> BannedUsers::bannedUsers() const
{
    return mBannedUsers;
}

void BannedUsers::setBannedUsers(const QList<BannedUser> &commands)
{
    mBannedUsers = commands;
}

void BannedUsers::parseBannedUsers(const QJsonObject &commandsObj)
{
    mBannedUsersCount = commandsObj["count"_L1].toInt();
    mOffset = commandsObj["offset"_L1].toInt();
    mTotal = commandsObj["total"_L1].toInt();
    mBannedUsers.clear();
    parseListBannedUsers(commandsObj);
}

int BannedUsers::offset() const
{
    return mOffset;
}

void BannedUsers::setOffset(int offset)
{
    mOffset = offset;
}

int BannedUsers::total() const
{
    return mTotal;
}

void BannedUsers::setTotal(int total)
{
    mTotal = total;
}

QDebug operator<<(QDebug d, const BannedUsers &t)
{
    d.space() << "total" << t.total();
    d.space() << "offset" << t.offset();
    d.space() << "bannedUsersCount" << t.bannedUsersCount() << "\n";
    for (int i = 0, total = t.bannedUsers().count(); i < total; ++i) {
        d.space() << t.bannedUsers().at(i) << "\n";
    }
    return d;
}
