/*
   Copyright (c) 2021 Laurent Montel <montel@kde.org>

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

#ifndef RETENTIONINFO_H
#define RETENTIONINFO_H

#include "libruqolacore_export.h"
#include <QDateTime>
#include <QDebug>
#include <QJsonObject>
class LIBRUQOLACORE_EXPORT RetentionInfo
{
public:
    RetentionInfo();
    ~RetentionInfo();
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
    Q_REQUIRED_RESULT bool operator==(const RetentionInfo &other) const;
    Q_REQUIRED_RESULT bool operator!=(const RetentionInfo &other) const;

private:
    int mMaxAge = -1;
    bool mEnabled = false;
    bool mOverrideGlobal = false;
    bool mExcludePinned = false;
    bool mFilesOnly = false;
};
Q_DECLARE_METATYPE(RetentionInfo)
Q_DECLARE_TYPEINFO(RetentionInfo, Q_MOVABLE_TYPE);
LIBRUQOLACORE_EXPORT QDebug operator<<(QDebug d, const RetentionInfo &t);
#endif // RETENTIONINFO_H
