/*
   Copyright (c) 2020 Alessandro Ambrosano <alessandro.ambrosano@gmail.com>

   This library is free software; you can redistribute it and/or modify
   it under the terms of the GNU Library General Public License as published
   by the Free Software Foundation; either version 2 of the License or
   ( at your option ) version 3 or, at the discretion of KDE e.V.
   ( which shall act as a proxy as in section 14 of the GPLv3 ), any later version.

   This library is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
   Library General Public License for more details.

   You should have received a copy of the GNU Library General Public License
   along with this library; see the file COPYING.LIB.  If not, write to
   the Free Software Foundation, Inc., 51 Franklin Street, Fifth Floor,
   Boston, MA 02110-1301, USA.
*/

#pragma once

#include <QAbstractListModel>
#include <QDir>
#include <QNetworkAccessManager>
#include <QNetworkReply>
#include <QNetworkRequest>
#include <QUrl>

#include "managerdatapaths.h"
#include "model/roommodel.h"
#include "rocketchataccount.h"
#include "ruqolaqmlplain_debug.h"

class RocketChatAccount;

class RoomAvatarsModel : public QAbstractListModel
{
    Q_OBJECT
  public:
    explicit RoomAvatarsModel(RocketChatAccount *account, QObject *parent = nullptr);
    ~RoomAvatarsModel() override;

    Q_REQUIRED_RESULT int rowCount(const QModelIndex &parent = QModelIndex()) const override;
    Q_REQUIRED_RESULT QVariant
    data(const QModelIndex &index, int role = Qt::DisplayRole) const override;
    Q_REQUIRED_RESULT QHash<int, QByteArray> roleNames() const override;

    void avatarDownloaded();

  private:
    RocketChatAccount *mAccount = nullptr;
    mutable QNetworkAccessManager mNetworkManager;
};

Q_DECLARE_METATYPE(RoomAvatarsModel *)
