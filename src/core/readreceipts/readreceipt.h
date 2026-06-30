/*
   SPDX-FileCopyrightText: 2026 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/
#pragma once

#include "libruqolacore_export.h"
class QJsonObject;
class QDebug;
class LIBRUQOLACORE_EXPORT ReadReceipt
{
public:
    ReadReceipt();
    ~ReadReceipt();

    [[nodiscard]] QByteArray userId() const;
    void setUserId(const QByteArray &newUserId);

    [[nodiscard]] qint64 timeStamp() const;
    void setTimeStamp(qint64 newTimeStamp);

    [[nodiscard]] qint64 updatedAt() const;
    void setUpdatedAt(qint64 newUpdatedAt);

    [[nodiscard]] bool operator==(const ReadReceipt &other) const;

    void parseReadReceiptInfo(const QJsonObject &obj);

    [[nodiscard]] bool isValid() const;

private:
    QByteArray mUserId;
    qint64 mTimeStamp = -1;
    qint64 mUpdatedAt = -1;
};

QT_DECL_METATYPE_EXTERN_TAGGED(ReadReceipt, Ruqola_ReadReceipt, LIBRUQOLACORE_EXPORT)
Q_DECLARE_TYPEINFO(ReadReceipt, Q_RELOCATABLE_TYPE);
LIBRUQOLACORE_EXPORT QDebug operator<<(QDebug d, const ReadReceipt &t);
