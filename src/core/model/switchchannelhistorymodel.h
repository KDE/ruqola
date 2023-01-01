/*
   SPDX-FileCopyrightText: 2022-2023 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#pragma once

#include <QAbstractItemModel>

#include "libruqolacore_export.h"

class LIBRUQOLACORE_EXPORT SwitchChannelHistoryModel : public QAbstractListModel
{
    Q_OBJECT
public:
    struct SwitchChannelInfo {
        SwitchChannelInfo() = default;
        SwitchChannelInfo(const QString &name, const QString &identifier)
            : mName(name)
            , mIdentifier(identifier)
        {
        }
        Q_REQUIRED_RESULT bool operator==(const SwitchChannelInfo &other) const;
        QString mName;
        QString mIdentifier;
    };

    explicit SwitchChannelHistoryModel(QObject *parent = nullptr);
    ~SwitchChannelHistoryModel() override;

    Q_REQUIRED_RESULT int rowCount(const QModelIndex &parent = QModelIndex()) const override;
    Q_REQUIRED_RESULT QVariant data(const QModelIndex &index, int role) const override;

    void addHistory(const QString &name, const QString &identifier);

    Q_REQUIRED_RESULT const QString identifier(int index);

private:
    QVector<SwitchChannelInfo> mSwichChannelsInfo;
};

Q_DECLARE_METATYPE(SwitchChannelHistoryModel::SwitchChannelInfo)
Q_DECLARE_TYPEINFO(SwitchChannelHistoryModel::SwitchChannelInfo, Q_MOVABLE_TYPE);
