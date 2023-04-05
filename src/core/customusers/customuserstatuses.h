/*
   SPDX-FileCopyrightText: 2020-2023 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#pragma once

#include "customuserstatus.h"
#include "libruqolacore_export.h"
#include <QDebug>
#include <QJsonObject>
#include <QVector>

class LIBRUQOLACORE_EXPORT CustomUserStatuses
{
public:
    CustomUserStatuses();
    ~CustomUserStatuses();

    Q_REQUIRED_RESULT bool isEmpty() const;
    void clear();
    Q_REQUIRED_RESULT int count() const;
    Q_REQUIRED_RESULT CustomUserStatus at(int index) const;

    Q_REQUIRED_RESULT int offset() const;
    void setOffset(int offset);

    Q_REQUIRED_RESULT int total() const;
    void setTotal(int total);

    Q_REQUIRED_RESULT QVector<CustomUserStatus> customUserses() const;
    void setCustomUserses(const QVector<CustomUserStatus> &customUserses);

    void parseCustomUserStatuses(const QJsonObject &customStatusObj);
    void deleteCustomUserStatuses(const QJsonArray &replyArray);
    void updateCustomUserStatues(const QJsonArray &replyArray);

private:
    LIBRUQOLACORE_NO_EXPORT void parseListCustomUserStatuses(const QJsonObject &commandsObj);
    QVector<CustomUserStatus> mCustomUserses;
    int mCustomUserCount = 0;
    int mOffset = 0;
    int mTotal = 0;
};
LIBRUQOLACORE_EXPORT QDebug operator<<(QDebug d, const CustomUserStatuses &t);
