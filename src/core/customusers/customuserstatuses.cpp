/*
   SPDX-FileCopyrightText: 2020-2026 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "customuserstatuses.h"

#include "ruqola_debug.h"

#include <QJsonArray>
#include <QJsonObject>

using namespace Qt::Literals::StringLiterals;
CustomUserStatuses::CustomUserStatuses() = default;

CustomUserStatuses::~CustomUserStatuses() = default;

void CustomUserStatuses::parseCustomUserStatuses(const QJsonObject &customStatusObj)
{
    mCustomUserCount = customStatusObj["count"_L1].toInt();
    mOffset = customStatusObj["offset"_L1].toInt();
    mTotal = customStatusObj["total"_L1].toInt();
    mCustomUserStatusList.clear();
    parseListCustomUserStatuses(customStatusObj);
}

void CustomUserStatuses::parseListCustomUserStatuses(const QJsonObject &customStatusObj)
{
    const QJsonArray customsUserArray = customStatusObj["statuses"_L1].toArray();
    mCustomUserStatusList.reserve(mCustomUserStatusList.count() + customsUserArray.count());
    for (const QJsonValue &current : customsUserArray) {
        if (current.type() == QJsonValue::Object) {
            const QJsonObject customUserObj = current.toObject();
            CustomUserStatus m;
            m.parseCustomStatus(customUserObj);
            mCustomUserStatusList.append(std::move(m));
        } else {
            qCWarning(RUQOLA_LOG) << "Problem when parsing customStatusUser" << current;
        }
    }
}

bool CustomUserStatuses::isEmpty() const
{
    return mCustomUserStatusList.isEmpty();
}

void CustomUserStatuses::clear()
{
    mCustomUserStatusList.clear();
}

int CustomUserStatuses::count() const
{
    return mCustomUserStatusList.count();
}

CustomUserStatus CustomUserStatuses::at(int index) const
{
    if (index < 0 || index >= mCustomUserStatusList.count()) {
        qCWarning(RUQOLA_LOG) << "Invalid index " << index;
        return {};
    }
    return mCustomUserStatusList.at(index);
}

int CustomUserStatuses::offset() const
{
    return mOffset;
}

void CustomUserStatuses::setOffset(int offset)
{
    mOffset = offset;
}

int CustomUserStatuses::total() const
{
    return mTotal;
}

void CustomUserStatuses::setTotal(int total)
{
    mTotal = total;
}

QList<CustomUserStatus> CustomUserStatuses::customUserStatusList() const
{
    return mCustomUserStatusList;
}

void CustomUserStatuses::setCustomUserStatusList(const QList<CustomUserStatus> &customUserses)
{
    mCustomUserStatusList = customUserses;
}

void CustomUserStatuses::deleteCustomUserStatuses(const QJsonArray &replyArray)
{
    for (int i = 0, total = replyArray.count(); i < total; ++i) {
        const QJsonObject obj = replyArray.at(i).toObject();
        const QJsonObject customStatusObj = obj.value("userStatusData"_L1).toObject();
        if (!customStatusObj.isEmpty()) {
            if (customStatusObj.contains("_id"_L1)) {
                const QByteArray identifier = customStatusObj.value("_id"_L1).toString().toLatin1();
                for (const CustomUserStatus &status : std::as_const(mCustomUserStatusList)) {
                    if (status.identifier() == identifier) {
                        mCustomUserStatusList.removeOne(status);
                        break;
                    }
                }
                // TODO fix total ???
            } else {
                qCWarning(RUQOLA_LOG) << "deleteCustomUser invalid QJsonObject" << customStatusObj;
            }
        }
    }
}

void CustomUserStatuses::updateCustomUserStatues(const QJsonArray &replyArray)
{
    // TODO fix total
    for (int i = 0; i < replyArray.count(); ++i) {
        const QJsonObject obj = replyArray.at(i).toObject();
        const QJsonObject customStatusObj = obj.value("userStatusData"_L1).toObject();
        if (!customStatusObj.isEmpty()) {
            if (customStatusObj.contains("_id"_L1)) {
                // previousStatusType
                // previousName
                //=> update otherwise add
                bool found = false;
                const QByteArray identifier = customStatusObj.value("_id"_L1).toString().toLatin1();
                for (CustomUserStatus &status : mCustomUserStatusList) {
                    if (status.identifier() == identifier) {
                        status.parseCustomStatus(customStatusObj);
                        found = true;
                        break;
                    }
                }
                if (!found) {
                    // Parse
                    CustomUserStatus newStatus;
                    newStatus.parseCustomStatus(customStatusObj, false);
                    if (newStatus.isValid()) {
                        mCustomUserStatusList.append(std::move(newStatus));
                    }
                }
            } else {
                // It's a bug in RC server see https://github.com/RocketChat/Rocket.Chat/issues/28542

                qCWarning(RUQOLA_LOG) << "updateCustomUser invalid QJsonObject" << customStatusObj;
            }
        }
    }
}

QDebug operator<<(QDebug d, const CustomUserStatuses &t)
{
    d.space() << "total" << t.total();
    d.space() << "offset" << t.offset();
    d.space() << "customStatus Count" << t.count() << "\n";
    for (int i = 0, total = t.customUserStatusList().count(); i < total; ++i) {
        d.space() << t.customUserStatusList().at(i) << "\n";
    }
    return d;
}
