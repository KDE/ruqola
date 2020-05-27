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

#include "roomavatarsmodel.h"

RoomAvatarsModel::RoomAvatarsModel(RocketChatAccount *account, QObject *parent)
    : QAbstractListModel(parent)
    , mAccount(account)
{
}

RoomAvatarsModel::~RoomAvatarsModel() = default;

int RoomAvatarsModel::rowCount(const QModelIndex &parent) const
{
    return 1;
}

QVariant RoomAvatarsModel::data(const QModelIndex &index, int role) const
{
    int roomNo = role - Qt::UserRole - 1;
    auto roomName = mAccount->roomModel()->index(roomNo, 0).data(RoomModel::RoomName).toString();

    QUrl cachePath = QUrl::fromUserInput(
        ManagerDataPaths::self()->path(ManagerDataPaths::Cache, mAccount->accountName()));
    cachePath.setPath(cachePath.path() + QStringLiteral("/avatar/") + roomName);

    if (QFileInfo(cachePath.toLocalFile()).exists()) {
        return cachePath;
    }

    QNetworkRequest req;
    req.setUrl(QUrl::fromUserInput(
        QStringLiteral("https://") + mAccount->serverUrl() + QStringLiteral("/avatar/") +
        roomName));
    req.setAttribute(QNetworkRequest::Attribute::User, roomNo);

    auto reply = mNetworkManager.get(req);
    connect(reply, &QNetworkReply::readyRead, this, &RoomAvatarsModel::avatarDownloaded);
    return {};
}

QHash<int, QByteArray> RoomAvatarsModel::roleNames() const
{
    int r = Qt::UserRole + 1;
    QHash<int, QByteArray> roles;
    for (int i = 0; i < mAccount->roomModel()->rowCount(); i++, r++) {
        roles[r] = mAccount->roomModel()
                       ->index(r - Qt::UserRole - 1, 0)
                       .data(RoomModel::RoomName)
                       .toString()
                       .toUtf8();
    }
    return roles;
}

void RoomAvatarsModel::avatarDownloaded()
{
    auto reply = qobject_cast<QNetworkReply *>(sender());

    int roomNo = reply->request().attribute(QNetworkRequest::Attribute::User).toInt();
    auto roomName = mAccount->roomModel()->index(roomNo, 0).data(RoomModel::RoomName).toString();
    QUrl cachePath = QUrl::fromUserInput(
        ManagerDataPaths::self()->path(ManagerDataPaths::Cache, mAccount->accountName()));
    cachePath.setPath(cachePath.path() + QStringLiteral("/avatar/") + roomName);

    auto buf = reply->readAll()
                   .replace(R"( viewBox="0 0 200 200")", "")
                   .replace(R"( width="100%" height="100%")", R"( width="200" height="200")")
                   .replace(R"( x="50%" y="50%" dy="0.36em")", R"( x="100" y="145")");
    const QUrl urldir = cachePath.adjusted(QUrl::RemoveFilename);
    QDir().mkpath(urldir.toLocalFile());
    QFile file(cachePath.toLocalFile());
    if (file.open(QIODevice::ReadWrite)) {
        file.write(buf);
        file.close();
    } else {
        qCWarning(RUQOLAQMLPLAIN_LOG) << " Error !" << file.errorString();
    }
    dataChanged(this->index(0, 0), this->index(0, 0), {roomNo});
}
