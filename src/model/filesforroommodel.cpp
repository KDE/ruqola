/*
   Copyright (c) 2018 Montel Laurent <montel@kde.org>

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

#include "filesforroommodel.h"
#include "rocketchataccount.h"
#include "ruqola_debug.h"


FilesForRoomModel::FilesForRoomModel(RocketChatAccount *account, QObject *parent)
    : QAbstractListModel(parent)
    , mRochetChantAccount(account)
{
}

FilesForRoomModel::~FilesForRoomModel()
{
}

void FilesForRoomModel::setFiles(const QVector<File> &files)
{
    if (rowCount() != 0) {
        beginRemoveRows(QModelIndex(), 0, mFiles.count() - 1);
        mFiles.clear();
        endRemoveRows();
    }
    if (!files.isEmpty()) {
        beginInsertRows(QModelIndex(), 0, files.count() - 1);
        mFiles = files;
        endInsertRows();
    }
}

int FilesForRoomModel::rowCount(const QModelIndex &parent) const
{
    Q_UNUSED(parent);
    return mFiles.count();
}

QVariant FilesForRoomModel::data(const QModelIndex &index, int role) const
{
    if (index.row() < 0 || index.row() >= mFiles.count()) {
        return QVariant();
    }

    const File file = mFiles.at(index.row());
    switch (role) {
    case UserName:
        return file.name();
    case UserId:
        return file.userId();
    case MimeType:
        return file.mimeType();
    case Url:
        return file.url();
    case Description:
        return file.description();
    case CanBeDeleted:
        return (mRochetChantAccount->userID() == file.userId());
    case FileId:
        return file.fileId();
    default:
        qCWarning(RUQOLA_LOG) << "Unknown filesmodel roles: " << role;
    }
    return {};
}

QHash<int, QByteArray> FilesForRoomModel::roleNames() const
{
    QHash<int, QByteArray> roles;
    roles[UserName] = QByteArrayLiteral("username");
    roles[UserId] = QByteArrayLiteral("userid");
    roles[MimeType] = QByteArrayLiteral("mimetype");
    roles[Url] = QByteArrayLiteral("url");    
    roles[Description] = QByteArrayLiteral("description");
    roles[CanBeDeleted] = QByteArrayLiteral("canbedeleted");
    roles[FileId] = QByteArrayLiteral("fileid");
    return roles;
}
