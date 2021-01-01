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

#include "customuserstatuses.h"
#include "ruqola_debug.h"

#include <QJsonArray>

CustomUserStatuses::CustomUserStatuses()
{
}

CustomUserStatuses::~CustomUserStatuses()
{
}

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
    for (int i = 0; i < replyArray.count(); ++i) {
        const QJsonObject obj = replyArray.at(i).toObject();
        const QJsonObject customStatusObj = obj.value(QLatin1String("userStatusData")).toObject();
        if (!customStatusObj.isEmpty()) {
            if (customStatusObj.contains(QLatin1String("_id"))) {
                const QString identifier = customStatusObj.value(QLatin1String("_id")).toString();
                for (const CustomUserStatus &status : qAsConst(mCustomUserses)) {
                    if (status.identifier() == identifier) {
                        mCustomUserses.removeOne(status);
                        break;
                    }
                }
                //TODO fix total ???
            } else {
                qCWarning(RUQOLA_LOG) << "deleteCustomUser invalid QJsonObject" << customStatusObj;
            }
        }
    }
}

void CustomUserStatuses::updateCustomUserStatues(const QJsonArray &replyArray)
{
    //TODO fix total
    for (int i = 0; i < replyArray.count(); ++i) {
        const QJsonObject obj = replyArray.at(i).toObject();
        const QJsonObject customStatusObj = obj.value(QLatin1String("userStatusData")).toObject();
        if (!customStatusObj.isEmpty()) {
            if (customStatusObj.contains(QLatin1String("_id"))) {
                //previousStatusType
                //previousName
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
                    //Parse
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

QDebug operator <<(QDebug d, const CustomUserStatuses &t)
{
    d << "total " << t.total();
    d << "offset " << t.offset();
    d << "customStatus Count " << t.count() << "\n";
    for (int i = 0, total = t.customUserses().count(); i < total; ++i) {
        d << t.customUserses().at(i) << "\n";
    }
    return d;
}
