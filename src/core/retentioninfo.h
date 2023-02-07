/*
   SPDX-FileCopyrightText: 2021-2023 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#pragma once

#include "libruqolacore_export.h"
#include <QDebug>
#include <QJsonObject>
class LIBRUQOLACORE_EXPORT RetentionInfo
{
public:
    RetentionInfo();
    ~RetentionInfo() = default;
    void parseRetentionInfo(const QJsonObject &replyObject);

    Q_REQUIRED_RESULT bool enabled() const;
    void setEnabled(bool enabled);

    Q_REQUIRED_RESULT bool overrideGlobal() const;
    void setOverrideGlobal(bool overrideGlobal);

    Q_REQUIRED_RESULT bool excludePinned() const;
    void setExcludePinned(bool excludePinned);

    Q_REQUIRED_RESULT bool filesOnly() const;
    void setFilesOnly(bool filesOnly);

    Q_REQUIRED_RESULT int maxAge() const;
    void setMaxAge(int maxAge);
    Q_REQUIRED_RESULT bool operator==(RetentionInfo other) const;
    Q_REQUIRED_RESULT bool operator!=(RetentionInfo other) const;

    Q_REQUIRED_RESULT static QJsonObject serialize(RetentionInfo retention);
    Q_REQUIRED_RESULT static RetentionInfo deserialize(const QJsonObject &o);
    Q_REQUIRED_RESULT bool isNotDefault() const;

private:
    int mMaxAge = -1;
    bool mEnabled = false;
    bool mOverrideGlobal = false;
    bool mExcludePinned = false;
    bool mFilesOnly = false;
};
Q_DECLARE_METATYPE(RetentionInfo)
Q_DECLARE_TYPEINFO(RetentionInfo, Q_MOVABLE_TYPE);
LIBRUQOLACORE_EXPORT QDebug operator<<(QDebug d, RetentionInfo t);
