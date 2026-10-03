/*
   SPDX-FileCopyrightText: 2022-2026 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#pragma once

#include "deviceinfo.h"
#include "libruqolacore_export.h"
#include "paginatedinfolist.h"
class QDebug;

class LIBRUQOLACORE_EXPORT DeviceInfos : public PaginatedInfoList<DeviceInfo, &DeviceInfo::parseDeviceInfo>
{
public:
    void parseDeviceInfos(const QJsonObject &obj);
    void parseMoreDeviceInfos(const QJsonObject &obj);
};
QT_DECL_METATYPE_EXTERN_TAGGED(DeviceInfos, Ruqola_DeviceInfos, LIBRUQOLACORE_EXPORT)
LIBRUQOLACORE_EXPORT QDebug operator<<(QDebug d, const DeviceInfos &t);
