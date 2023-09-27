/*
   SPDX-FileCopyrightText: 2020-2023 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#pragma once

#include "custombasemodel.h"
#include "libruqolacore_export.h"
#include "roominfo/roomsinfo.h"

class LIBRUQOLACORE_EXPORT AdminRoomsModel : public CustomBaseModel
{
    Q_OBJECT
public:
    enum AdminRoomsRoles {
        Name,
        Topic,
        ChannelType,
        ChannelTypeStr,
        MessagesCount,
        UsersCount,
        ReadOnly,
        DefaultRoom,
        Featured,
        Identifier,
        IsTeam,
        LastColumn = IsTeam,
    };
    Q_ENUM(AdminRoomsRoles)

    explicit AdminRoomsModel(QObject *parent = nullptr);
    ~AdminRoomsModel() override;

    [[nodiscard]] int rowCount(const QModelIndex &parent = QModelIndex()) const override;
    [[nodiscard]] QVariant data(const QModelIndex &index, int role) const override;
    [[nodiscard]] QVariant headerData(int section, Qt::Orientation orientation, int role) const override;
    [[nodiscard]] int columnCount(const QModelIndex &parent = QModelIndex()) const override;

    [[nodiscard]] RoomsInfo adminRooms() const;

    void setAdminRooms(const RoomsInfo &adminrooms);

    [[nodiscard]] int total() const override;
    void parseElements(const QJsonObject &obj) override;
    void addMoreElements(const QJsonObject &obj) override;
    [[nodiscard]] QList<int> hideColumns() const override;

    void removeElement(const QString &identifier) override;

    void updateElement(const QJsonObject &obj) override;

private:
    Q_DISABLE_COPY(AdminRoomsModel)
    LIBRUQOLACORE_NO_EXPORT void checkFullList() override;
    RoomsInfo mAdminRooms;
};
