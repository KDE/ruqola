/*
   SPDX-FileCopyrightText: 2022-2026 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "deviceinfos.h"

#include <QDebug>

QT_IMPL_METATYPE_EXTERN_TAGGED(DeviceInfos, Ruqola_DeviceInfos)

using namespace Qt::Literals::StringLiterals;

void DeviceInfos::parseDeviceInfos(const QJsonObject &obj)
{
    parseInfos(obj, "sessions"_L1);
}

void DeviceInfos::parseMoreDeviceInfos(const QJsonObject &obj)
{
    parseMoreInfos(obj, "sessions"_L1);
}

QDebug operator<<(QDebug d, const DeviceInfos &t)
{
    d.space() << "total" << t.total();
    d.space() << "offset" << t.offset();
    d.space() << "loadedCount" << t.loadedCount() << "\n";
    for (const DeviceInfo &info : t.list()) {
        d.space() << info << "\n";
    }
    return d;
}
