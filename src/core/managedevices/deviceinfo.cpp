/*
   SPDX-FileCopyrightText: 2022 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "deviceinfo.h"

DeviceInfo::DeviceInfo() = default;

DeviceInfo::~DeviceInfo() = default;

QDebug operator<<(QDebug d, const DeviceInfo &t)
{
    return d;
}
