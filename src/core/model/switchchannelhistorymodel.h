/*
   SPDX-FileCopyrightText: 2022-2024 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#pragma once

#include <QAbstractItemModel>

#include "libruqolacore_export.h"

class LIBRUQOLACORE_EXPORT SwitchChannelHistoryModel : public QAbstractListModel
{
    Q_OBJECT
public:
    enum SwitchChannelHistoryRoles {
        Name = Qt::UserRole + 1,
        Identifier,
        AvatarInfo,
    };
    Q_ENUM(SwitchChannelHistoryRoles)

    struct SwitchChannelInfo {
        SwitchChannelInfo() = default;
        SwitchChannelInfo(const QString &name, const QString &identifier)
            : mName(name)
            , mIdentifier(identifier)
        {
        }
        [[nodiscard]] bool operator==(const SwitchChannelInfo &other) const;
        QString mName;
        QString mIdentifier;
    };

    explicit SwitchChannelHistoryModel(QObject *parent = nullptr);
    ~SwitchChannelHistoryModel() override;

    [[nodiscard]] int rowCount(const QModelIndex &parent = QModelIndex()) const override;
    [[nodiscard]] QVariant data(const QModelIndex &index, int role) const override;

    void addHistory(const SwitchChannelInfo &info);

    [[nodiscard]] const QString identifier(int index);

private:
    QVector<SwitchChannelInfo> mSwichChannelsInfo;
};

Q_DECLARE_METATYPE(SwitchChannelHistoryModel::SwitchChannelInfo)
Q_DECLARE_TYPEINFO(SwitchChannelHistoryModel::SwitchChannelInfo, Q_MOVABLE_TYPE);
