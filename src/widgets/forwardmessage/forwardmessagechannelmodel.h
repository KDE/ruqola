/*
  SPDX-FileCopyrightText: 2024 Laurent Montel <montel.org>

  SPDX-License-Identifier: GPL-2.0-or-later
*/

#pragma once

#include "channelusercompleter.h"
#include <QAbstractListModel>

class ForwardMessageChannelModel : public QAbstractListModel
{
    Q_OBJECT
public:
    enum ForwardMessageChannelRoles {
        Name = Qt::UserRole + 1,
        ChannelId,
        Icon,
    };
    Q_ENUM(ForwardMessageChannelRoles)

    explicit ForwardMessageChannelModel(QObject *parent = nullptr);
    ~ForwardMessageChannelModel() override;

    [[nodiscard]] int rowCount(const QModelIndex &parent = QModelIndex()) const override;
    [[nodiscard]] QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const override;

    void setRooms(const QVector<ChannelUserCompleter> &rooms);

    void clear();

private:
    QVector<ChannelUserCompleter> mRooms;
};
