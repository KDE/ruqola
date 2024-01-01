/*
   SPDX-FileCopyrightText: 2022-2024 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#pragma once

#include "custombasemodel.h"
#include "libruqolacore_export.h"
#include "managedevices/deviceinfos.h"

class LIBRUQOLACORE_EXPORT DeviceInfoModel : public CustomBaseModel
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

    [[nodiscard]] int rowCount(const QModelIndex &parent = QModelIndex()) const override;
    [[nodiscard]] QVariant data(const QModelIndex &index, int role) const override;
    [[nodiscard]] QVariant headerData(int section, Qt::Orientation orientation, int role) const override;
    [[nodiscard]] int columnCount(const QModelIndex &parent = QModelIndex()) const override;

    [[nodiscard]] int total() const override;
    void parseElements(const QJsonObject &obj) override;
    void addMoreElements(const QJsonObject &obj) override;
    [[nodiscard]] QList<int> hideColumns() const override;

    void removeElement(const QString &identifier) override;

    [[nodiscard]] const DeviceInfos &deviceInfos() const;
    void setDeviceInfos(const DeviceInfos &newDeviceInfos);

private:
    LIBRUQOLACORE_NO_EXPORT void clear();
    LIBRUQOLACORE_NO_EXPORT void checkFullList() override;
    DeviceInfos mDeviceInfos;
};
