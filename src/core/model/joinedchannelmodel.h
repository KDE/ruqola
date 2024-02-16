/*
  SPDX-FileCopyrightText: 2024 Laurent Montel <montel.org>

  SPDX-License-Identifier: GPL-2.0-or-later
*/

#pragma once

#include "channelusercompleter.h"
#include "libruqolacore_export.h"
#include <QAbstractListModel>

class LIBRUQOLACORE_EXPORT JoinedChannelModel : public QAbstractListModel
{
    Q_OBJECT
public:
    enum ForwardMessageChannelRoles {
        Name = Qt::UserRole + 1,
        ChannelId,
        AvatarInfo,
    };
    Q_ENUM(ForwardMessageChannelRoles)

    explicit JoinedChannelModel(QObject *parent = nullptr);
    ~JoinedChannelModel() override;

    [[nodiscard]] int rowCount(const QModelIndex &parent = QModelIndex()) const override;
    [[nodiscard]] QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const override;

    void setRooms(const QList<ChannelUserCompleter> &rooms);

    void clear();

private:
    QList<ChannelUserCompleter> mRooms;
};
