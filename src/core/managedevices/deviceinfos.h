/*
   SPDX-FileCopyrightText: 2022 Laurent Montel <montel@kde.org>

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

    Q_REQUIRED_RESULT int offset() const;
    void setOffset(int newOffset);

    Q_REQUIRED_RESULT int total() const;
    void setTotal(int newTotal);

    Q_REQUIRED_RESULT int deviceInfosCount() const;
    void setDeviceInfosCount(int newDeviceInfosCount);

    Q_REQUIRED_RESULT const QVector<DeviceInfo> &deviceInfosList() const;
    void setDeviceInfosList(const QVector<DeviceInfo> &newDeviceInfosList);

    Q_REQUIRED_RESULT bool isEmpty() const;
    void clear();
    Q_REQUIRED_RESULT int count() const;
    Q_REQUIRED_RESULT DeviceInfo at(int index) const;

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
