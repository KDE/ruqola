/*
   SPDX-FileCopyrightText: 2023-2026 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#pragma once
#include "libruqolacore_export.h"
#include <QDateTime>

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

    [[nodiscard]] QByteArray identifier() const;

    [[nodiscard]] QString dateTimeStr() const;

private:
    LIBRUQOLACORE_NO_EXPORT void createUniqueIdentifier();
    static quint64 identifierId;
    QString mAccountName;
    QString mMessage;
    QDateTime mDateTime;
    QByteArray mIdentifier;
    QString mDateTimeStr;
};
Q_DECLARE_TYPEINFO(ServerErrorInfo, Q_RELOCATABLE_TYPE);
class QDebug;
LIBRUQOLACORE_EXPORT QDebug operator<<(QDebug d, const ServerErrorInfo &t);
