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

#include "filesmodelforroom.h"
#include "ruqola_debug.h"

FilesModelForRoom::FilesModelForRoom(QObject *parent)
    : QAbstractListModel(parent)
{

}

FilesModelForRoom::~FilesModelForRoom()
{

}

void FilesModelForRoom::insertFiles(const QVector<File> &files)
{
    mFiles.clear();
    beginInsertRows(QModelIndex(), 0, rowCount() - 1);
    mFiles = files;
    endInsertRows();
}

int FilesModelForRoom::rowCount(const QModelIndex &parent) const
{
    Q_UNUSED(parent);
    return mFiles.count();
}

QVariant FilesModelForRoom::data(const QModelIndex &index, int role) const
{
    const File user = mFiles.at(index.row());
    switch (role) {
    case UserName:
        return user.name();
    case UserId:
        return user.userId();
    case MimeType:
        return user.mimeType();
    case Url:
        return user.url();
    case Description:
        return user.description();

    default:
        qCWarning(RUQOLA_LOG) << "Unknown filesmodel roles: " << role;
    }
    return {};
}

QHash<int, QByteArray> FilesModelForRoom::roleNames() const
{
    QHash<int, QByteArray> roles;
    roles[UserName] = QByteArrayLiteral("username");
    roles[UserId] = QByteArrayLiteral("userid");
    roles[MimeType] = QByteArrayLiteral("mimetype");
    roles[Url] = QByteArrayLiteral("url");
    roles[Description] = QByteArrayLiteral("description");
    return roles;
}
