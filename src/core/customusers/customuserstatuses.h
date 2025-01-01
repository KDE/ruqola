/*
   SPDX-FileCopyrightText: 2020-2025 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#pragma once

#include "customuserstatus.h"
#include "libruqolacore_export.h"
#include <QDebug>
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

    [[nodiscard]] QList<CustomUserStatus> customUserses() const;
    void setCustomUserses(const QList<CustomUserStatus> &customUserses);

    void parseCustomUserStatuses(const QJsonObject &customStatusObj);
    void deleteCustomUserStatuses(const QJsonArray &replyArray);
    void updateCustomUserStatues(const QJsonArray &replyArray);

private:
    LIBRUQOLACORE_NO_EXPORT void parseListCustomUserStatuses(const QJsonObject &commandsObj);
    QList<CustomUserStatus> mCustomUserses;
    int mCustomUserCount = 0;
    int mOffset = 0;
    int mTotal = 0;
};
LIBRUQOLACORE_EXPORT QDebug operator<<(QDebug d, const CustomUserStatuses &t);
