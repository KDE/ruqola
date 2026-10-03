/*
   SPDX-FileCopyrightText: 2020-2026 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "customuserstatuses.h"

#include "ruqola_debug.h"

using namespace Qt::Literals::StringLiterals;

void CustomUserStatuses::parseCustomUserStatuses(const QJsonObject &obj)
{
    parseInfos(obj, "statuses"_L1);
}

void CustomUserStatuses::deleteCustomUserStatuses(const QJsonArray &replyArray)
{
    for (int i = 0, total = replyArray.count(); i < total; ++i) {
        const QJsonObject obj = replyArray.at(i).toObject();
        const QJsonObject customStatusObj = obj.value("userStatusData"_L1).toObject();
        if (!customStatusObj.isEmpty()) {
            if (customStatusObj.contains("_id"_L1)) {
                const QByteArray identifier = customStatusObj.value("_id"_L1).toString().toLatin1();
                for (const CustomUserStatus &status : std::as_const(mList)) {
                    if (status.identifier() == identifier) {
                        mList.removeOne(status);
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
                for (CustomUserStatus &status : mList) {
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
                        mList.append(std::move(newStatus));
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
    for (const CustomUserStatus &status : t.list()) {
        d.space() << status << "\n";
    }
    return d;
}
