/*
   SPDX-FileCopyrightText: 2023-2024 Laurent Montel <montel.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#pragma once
#include "libruqolacore_export.h"
#include <QDateTime>
#include <QDebug>

class LIBRUQOLACORE_EXPORT ServerErrorInfo
{
    Q_GADGET
public:
    ServerErrorInfo();
    ~ServerErrorInfo();

    [[nodiscard]] QString accountName() const;
    void setAccountName(const QString &newAccountName);

    [[nodiscard]] QString message() const;
    void setMessage(const QString &newMessage);

    [[nodiscard]] QDateTime dateTime() const;
    void setDateTime(const QDateTime &newDateTime);

    [[nodiscard]] QString identifier() const;

    [[nodiscard]] QString dateTimeStr() const;

private:
    LIBRUQOLACORE_NO_EXPORT void createUniqueIdentifier();
    static quint64 identifierId;
    QString mAccountName;
    QString mMessage;
    QDateTime mDateTime;
    QString mIdentifier;
    QString mDateTimeStr;
};
#if QT_VERSION < QT_VERSION_CHECK(6, 0, 0)
Q_DECLARE_METATYPE(ServerErrorInfo)
#endif
Q_DECLARE_TYPEINFO(ServerErrorInfo, Q_MOVABLE_TYPE);
LIBRUQOLACORE_EXPORT QDebug operator<<(QDebug d, const ServerErrorInfo &t);
