/*
   SPDX-FileCopyrightText: 2023 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "servererrorinfo.h"

ServerErrorInfo::ServerErrorInfo() = default;

ServerErrorInfo::~ServerErrorInfo() = default;

QDebug operator<<(QDebug d, const ServerErrorInfo &t)
{
    // TODO
    return d;
}
