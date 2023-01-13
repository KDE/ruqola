/*
   SPDX-FileCopyrightText: 2023 Laurent Montel <montel@kde.org>

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
};

Q_DECLARE_METATYPE(ServerErrorInfo)
Q_DECLARE_TYPEINFO(ServerErrorInfo, Q_MOVABLE_TYPE);
LIBRUQOLACORE_EXPORT QDebug operator<<(QDebug d, const ServerErrorInfo &t);
