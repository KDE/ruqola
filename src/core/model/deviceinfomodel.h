/*
   SPDX-FileCopyrightText: 2022-2023 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#pragma once

#include "directorybasemodel.h"
#include "libruqolacore_export.h"
#include "managedevices/deviceinfos.h"

class LIBRUQOLACORE_EXPORT DeviceInfoModel : public DirectoryBaseModel
{
    Q_OBJECT
public:
    enum DeviceInfoRoles {
        Os,
        Client,
        Host,
        Identifier,
        SessionId,
        Ip,
        UserId,
        LoginAt,
        LastColumn = LoginAt,
    };
    Q_ENUM(DeviceInfoRoles)

    explicit DeviceInfoModel(QObject *parent = nullptr);
    ~DeviceInfoModel() override;

    Q_REQUIRED_RESULT int rowCount(const QModelIndex &parent = QModelIndex()) const override;
    Q_REQUIRED_RESULT QVariant data(const QModelIndex &index, int role) const override;
    Q_REQUIRED_RESULT QVariant headerData(int section, Qt::Orientation orientation, int role) const override;
    Q_REQUIRED_RESULT int columnCount(const QModelIndex &parent = QModelIndex()) const override;

    Q_REQUIRED_RESULT int total() const override;
    void parseElements(const QJsonObject &obj) override;
    void addMoreElements(const QJsonObject &obj) override;
    Q_REQUIRED_RESULT QList<int> hideColumns() const override;

    void removeElement(const QString &identifier) override;

    Q_REQUIRED_RESULT const DeviceInfos &deviceInfos() const;
    void setDeviceInfos(const DeviceInfos &newDeviceInfos);

private:
    Q_DISABLE_COPY(DeviceInfoModel)
    LIBRUQOLACORE_NO_EXPORT void checkFullList();
    DeviceInfos mDeviceInfos;
};
