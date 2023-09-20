/*
   SPDX-FileCopyrightText: 2021-2023 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#pragma once

#include "custombasemodel.h"
#include "libruqolacore_export.h"
#include "roominfo/roomsinfo.h"
class LIBRUQOLACORE_EXPORT DirectoryRoomsModel : public CustomBaseModel
{
    Q_OBJECT
public:
    enum DirectoryRoomsRoles {
        Name,
        Topic,
        UsersCount,
        ReadOnly,
        DefaultRoom,
        Identifier,
        ChannelType,
        ChannelTypeStr,
        LastMessageDateTime,
        CreatedDateTime,
        LastMessageStr,
        BelongsTo,
        CreatedStr,
        LastColumn = CreatedStr,
    };
    Q_ENUM(DirectoryRoomsRoles)

    explicit DirectoryRoomsModel(QObject *parent = nullptr);
    ~DirectoryRoomsModel() override;

    Q_REQUIRED_RESULT int rowCount(const QModelIndex &parent = QModelIndex()) const override;
    Q_REQUIRED_RESULT QVariant data(const QModelIndex &index, int role) const override;
    Q_REQUIRED_RESULT QVariant headerData(int section, Qt::Orientation orientation, int role) const override;
    Q_REQUIRED_RESULT int columnCount(const QModelIndex &parent = {}) const override;
    Q_REQUIRED_RESULT int total() const override;

    void parseElements(const QJsonObject &roomsObj) override;

    void addMoreElements(const QJsonObject &discussionsObj) override;
    Q_REQUIRED_RESULT QList<int> excludeResizeToContentColumns() const override;

protected:
    Q_REQUIRED_RESULT QList<int> hideColumns() const override;

private:
    Q_DISABLE_COPY(DirectoryRoomsModel)
    LIBRUQOLACORE_NO_EXPORT void checkFullList();
    RoomsInfo mRoomsInfo;
};
