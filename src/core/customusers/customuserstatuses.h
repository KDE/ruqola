/*
   SPDX-FileCopyrightText: 2020-2025 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#pragma once

#include "customuserstatus.h"
#include "libruqolacore_export.h"
#include <QList>

class LIBRUQOLACORE_EXPORT CustomUserStatuses
{
public:
    CustomUserStatuses();
    ~CustomUserStatuses();

    [[nodiscard]] bool isEmpty() const;
    void clear();
    [[nodiscard]] int count() const;
    [[nodiscard]] CustomUserStatus at(int index) const;

    [[nodiscard]] int offset() const;
    void setOffset(int offset);

    [[nodiscard]] int total() const;
    void setTotal(int total);

    [[nodiscard]] QList<CustomUserStatus> customUserStatusList() const;
    void setCustomUserStatusList(const QList<CustomUserStatus> &customUserses);

    void parseCustomUserStatuses(const QJsonObject &customStatusObj);
    void deleteCustomUserStatuses(const QJsonArray &replyArray);
    void updateCustomUserStatues(const QJsonArray &replyArray);

private:
    LIBRUQOLACORE_NO_EXPORT void parseListCustomUserStatuses(const QJsonObject &commandsObj);
    QList<CustomUserStatus> mCustomUserStatusList;
    int mCustomUserCount = 0;
    int mOffset = 0;
    int mTotal = 0;
};
class QDebug;
LIBRUQOLACORE_EXPORT QDebug operator<<(QDebug d, const CustomUserStatuses &t);
