/*
  SPDX-FileCopyrightText: 2024 Laurent Montel <montel.org>

  SPDX-License-Identifier: GPL-2.0-or-later
*/

#pragma once

#include <QAbstractListModel>

class ForwardMessageChannelModel : public QAbstractListModel
{
    Q_OBJECT
public:
    explicit ForwardMessageChannelModel(QObject *parent = nullptr);
    ~ForwardMessageChannelModel() override;

    [[nodiscard]] int rowCount(const QModelIndex &parent = QModelIndex()) const override;
    [[nodiscard]] QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const override;

    // void setRooms(const QVector<TeamRoomCompleter> &rooms);

private:
    // QVector<TeamRoomCompleter> mRooms;
};
