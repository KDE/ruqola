/*
   SPDX-FileCopyrightText: 2021-2023 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#pragma once

#include "channel.h"
#include "libruqolacore_export.h"
#include <QAbstractListModel>

class LIBRUQOLACORE_EXPORT ChannelCompleterModel : public QAbstractListModel
{
    Q_OBJECT
public:
    enum ChannelRoles {
        RoomName = Qt::UserRole + 1,
        ChannelId,
    };
    Q_ENUM(ChannelRoles)

    explicit ChannelCompleterModel(QObject *parent = nullptr);
    ~ChannelCompleterModel() override;

    [[nodiscard]] int rowCount(const QModelIndex &parent = QModelIndex()) const override;
    [[nodiscard]] QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const override;

    void clear();
    void insertChannels(const QVector<Channel> &users);

private:
    Q_DISABLE_COPY(ChannelCompleterModel)
    QVector<Channel> mChannels;
};
