/*
   SPDX-FileCopyrightText: 2026 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/
#pragma once

#include "libruqolacore_export.h"
class QDebug;
class LIBRUQOLACORE_EXPORT ReadReceipt
{
public:
    ReadReceipt();
    ~ReadReceipt();

    [[nodiscard]] QByteArray userId() const;
    void setUserId(const QByteArray &newUserId);

private:
    QByteArray mUserId;
};

QT_DECL_METATYPE_EXTERN_TAGGED(ReadReceipt, Ruqola_ReadReceipt, LIBRUQOLACORE_EXPORT)
Q_DECLARE_TYPEINFO(ReadReceipt, Q_RELOCATABLE_TYPE);
LIBRUQOLACORE_EXPORT QDebug operator<<(QDebug d, const ReadReceipt &t);
