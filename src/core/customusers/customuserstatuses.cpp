/*
   SPDX-FileCopyrightText: 2020-2021 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "customuserstatuses.h"
#include "ruqola_debug.h"

#include <QJsonArray>

CustomUserStatuses::CustomUserStatuses() = default;

CustomUserStatuses::~CustomUserStatuses() = default;

void CustomUserStatuses::parseCustomUserStatuses(const QJsonObject &customStatusObj)
{
    mCustomUserCount = customStatusObj[QStringLiteral("count")].toInt();
    mOffset = customStatusObj[QStringLiteral("offset")].toInt();
    mTotal = customStatusObj[QStringLiteral("total")].toInt();
    mCustomUserses.clear();
    parseListCustomUserStatuses(customStatusObj);
}

void CustomUserStatuses::parseListCustomUserStatuses(const QJsonObject &customStatusObj)
{
    const QJsonArray customsUserArray = customStatusObj[QStringLiteral("statuses")].toArray();
    mCustomUserses.reserve(mCustomUserses.count() + customsUserArray.count());
    for (const QJsonValue &current : customsUserArray) {
        if (current.type() == QJsonValue::Object) {
            const QJsonObject customUserObj = current.toObject();
            CustomUserStatus m;
            m.parseCustomStatus(customUserObj);
            mCustomUserses.append(m);
        } else {
            qCWarning(RUQOLA_LOG) << "Problem when parsing customStatusUser" << current;
        }
    }
}

bool CustomUserStatuses::isEmpty() const
{
    return mCustomUserses.isEmpty();
}

void CustomUserStatuses::clear()
{
    mCustomUserses.clear();
}

int CustomUserStatuses::count() const
{
    return mCustomUserses.count();
}

CustomUserStatus CustomUserStatuses::at(int index) const
{
    return mCustomUserses.at(index);
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

QVector<CustomUserStatus> CustomUserStatuses::customUserses() const
{
    return mCustomUserses;
}

void CustomUserStatuses::setCustomUserses(const QVector<CustomUserStatus> &customUserses)
{
    mCustomUserses = customUserses;
}

void CustomUserStatuses::deleteCustomUserStatuses(const QJsonArray &replyArray)
{
    for (int i = 0, total = replyArray.count(); i < total; ++i) {
        const QJsonObject obj = replyArray.at(i).toObject();
        const QJsonObject customStatusObj = obj.value(QLatin1String("userStatusData")).toObject();
        if (!customStatusObj.isEmpty()) {
            if (customStatusObj.contains(QLatin1String("_id"))) {
                const QString identifier = customStatusObj.value(QLatin1String("_id")).toString();
                for (const CustomUserStatus &status : std::as_const(mCustomUserses)) {
                    if (status.identifier() == identifier) {
                        mCustomUserses.removeOne(status);
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
        const QJsonObject customStatusObj = obj.value(QLatin1String("userStatusData")).toObject();
        if (!customStatusObj.isEmpty()) {
            if (customStatusObj.contains(QLatin1String("_id"))) {
                // previousStatusType
                // previousName
                //=> update otherwise add
                bool found = false;
                const QString identifier = customStatusObj.value(QLatin1String("_id")).toString();
                for (CustomUserStatus &status : mCustomUserses) {
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
                        mCustomUserses.append(newStatus);
                    }
                }
            } else {
                qCWarning(RUQOLA_LOG) << "updateCustomUser invalid QJsonObject" << customStatusObj;
            }
        }
    }
}

QDebug operator<<(QDebug d, const CustomUserStatuses &t)
{
    d << "total " << t.total();
    d << "offset " << t.offset();
    d << "customStatus Count " << t.count() << "\n";
    for (int i = 0, total = t.customUserses().count(); i < total; ++i) {
        d << t.customUserses().at(i) << "\n";
    }
    return d;
}
