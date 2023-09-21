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

    [[nodiscard]] bool enabled() const;
    void setEnabled(bool enabled);

    [[nodiscard]] bool overrideGlobal() const;
    void setOverrideGlobal(bool overrideGlobal);

    [[nodiscard]] bool excludePinned() const;
    void setExcludePinned(bool excludePinned);

    [[nodiscard]] bool filesOnly() const;
    void setFilesOnly(bool filesOnly);

    [[nodiscard]] int maxAge() const;
    void setMaxAge(int maxAge);
    [[nodiscard]] bool operator==(RetentionInfo other) const;
    [[nodiscard]] bool operator!=(RetentionInfo other) const;

    [[nodiscard]] static QJsonObject serialize(RetentionInfo retention);
    [[nodiscard]] static RetentionInfo deserialize(const QJsonObject &o);
    [[nodiscard]] bool isNotDefault() const;

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
