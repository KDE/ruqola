/*
   SPDX-FileCopyrightText: 2022-2023 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#pragma once

#include "deviceinfo.h"
#include "libruqolacore_export.h"
#include <QDebug>
#include <QJsonObject>
#include <QString>
#include <QVector>

class LIBRUQOLACORE_EXPORT DeviceInfos
{
public:
    DeviceInfos();

    [[nodiscard]] int offset() const;
    void setOffset(int newOffset);

    [[nodiscard]] int total() const;
    void setTotal(int newTotal);

    [[nodiscard]] int deviceInfosCount() const;
    void setDeviceInfosCount(int newDeviceInfosCount);

    [[nodiscard]] const QVector<DeviceInfo> &deviceInfosList() const;
    void setDeviceInfosList(const QVector<DeviceInfo> &newDeviceInfosList);

    [[nodiscard]] bool isEmpty() const;
    void clear();
    [[nodiscard]] int count() const;
    [[nodiscard]] DeviceInfo at(int index) const;

    void parseDeviceInfos(const QJsonObject &discussionsObj);
    void parseMoreDeviceInfos(const QJsonObject &deviceInfosObj);

    DeviceInfo takeAt(int index);

private:
    void parseDeviceInfosObj(const QJsonObject &discussionsObj);
    QVector<DeviceInfo> mDeviceInfosList;
    int mDeviceInfosCount = 0;
    int mOffset = 0;
    int mTotal = 0;
};
Q_DECLARE_METATYPE(DeviceInfos)
LIBRUQOLACORE_EXPORT QDebug operator<<(QDebug d, const DeviceInfos &t);
