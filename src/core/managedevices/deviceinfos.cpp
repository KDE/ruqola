/*
   SPDX-FileCopyrightText: 2022-2025 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "deviceinfos.h"

#include "ruqola_debug.h"

#include <QJsonArray>
#include <QJsonObject>

using namespace Qt::Literals::StringLiterals;
DeviceInfos::DeviceInfos() = default;

QDebug operator<<(QDebug d, const DeviceInfos &t)
{
    d.space() << "total" << t.total();
    d.space() << "offset" << t.offset();
    d.space() << "deviceInfosCount" << t.deviceInfosCount() << "\n";
    for (int i = 0, total = t.deviceInfosList().count(); i < total; ++i) {
        d.space() << t.deviceInfosList().at(i) << "\n";
    }
    return d;
}

int DeviceInfos::offset() const
{
    return mOffset;
}

void DeviceInfos::setOffset(int newOffset)
{
    mOffset = newOffset;
}

int DeviceInfos::total() const
{
    return mTotal;
}

void DeviceInfos::setTotal(int newTotal)
{
    mTotal = newTotal;
}

int DeviceInfos::deviceInfosCount() const
{
    return mDeviceInfosCount;
}

void DeviceInfos::setDeviceInfosCount(int newDeviceInfosCount)
{
    mDeviceInfosCount = newDeviceInfosCount;
}

const QList<DeviceInfo> &DeviceInfos::deviceInfosList() const
{
    return mDeviceInfosList;
}

void DeviceInfos::setDeviceInfosList(const QList<DeviceInfo> &newDeviceInfosList)
{
    mDeviceInfosList = newDeviceInfosList;
}

bool DeviceInfos::isEmpty() const
{
    return mDeviceInfosList.isEmpty();
}

void DeviceInfos::clear()
{
    mDeviceInfosList.clear();
}

int DeviceInfos::count() const
{
    return mDeviceInfosList.count();
}

DeviceInfo DeviceInfos::at(int index) const
{
    if (index < 0 || index >= mDeviceInfosList.count()) {
        qCWarning(RUQOLA_LOG) << "Invalid index " << index;
        return {};
    }
    return mDeviceInfosList.at(index);
}

void DeviceInfos::parseDeviceInfos(const QJsonObject &deviceInfosObj)
{
    mDeviceInfosList.clear();
    mDeviceInfosCount = deviceInfosObj["count"_L1].toInt();
    mOffset = deviceInfosObj["offset"_L1].toInt();
    mTotal = deviceInfosObj["total"_L1].toInt();
    mDeviceInfosList.reserve(mDeviceInfosCount);
    parseDeviceInfosObj(deviceInfosObj);
}

void DeviceInfos::parseDeviceInfosObj(const QJsonObject &deviceInfosObj)
{
    const QJsonArray discussionsArray = deviceInfosObj["sessions"_L1].toArray();
    for (const QJsonValue &current : discussionsArray) {
        if (current.type() == QJsonValue::Object) {
            const QJsonObject discussionObject = current.toObject();
            DeviceInfo m;
            m.parseDeviceInfo(discussionObject);
            mDeviceInfosList.append(std::move(m));
        } else {
            qCWarning(RUQOLA_LOG) << "Problem when parsing device infos" << current;
        }
    }
}

void DeviceInfos::parseMoreDeviceInfos(const QJsonObject &deviceInfosObj)
{
    const int deviceInfosCount = deviceInfosObj["count"_L1].toInt();
    mOffset = deviceInfosObj["offset"_L1].toInt();
    mTotal = deviceInfosObj["total"_L1].toInt();
    parseDeviceInfosObj(deviceInfosObj);
    mDeviceInfosCount += deviceInfosCount;
}

DeviceInfo DeviceInfos::takeAt(int index)
{
    return mDeviceInfosList.takeAt(index);
}
