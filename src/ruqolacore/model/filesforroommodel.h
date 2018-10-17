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

#ifndef FILESMODELFORROOM_H
#define FILESMODELFORROOM_H

#include <QAbstractListModel>
#include "file.h"
#include "libruqola_private_export.h"
class RocketChatAccount;
class LIBRUQOLACORE_TESTS_EXPORT FilesForRoomModel : public QAbstractListModel
{
    Q_OBJECT
public:
    enum UserRoles {
        FileName = Qt::UserRole + 1,
        UserId,
        Description,
        Url,
        MimeType,
        CanBeDeleted,
        FileId,
        TimeStamp
    };
    Q_ENUM(UserRoles)

    explicit FilesForRoomModel(RocketChatAccount *account = nullptr, QObject *parent = nullptr);
    ~FilesForRoomModel() override;

    Q_REQUIRED_RESULT Q_INVOKABLE int rowCount(const QModelIndex &parent = QModelIndex()) const override;
    Q_REQUIRED_RESULT QVariant data(const QModelIndex &index, int role) const override;

    void setFiles(const QVector<File> &files);

    Q_REQUIRED_RESULT QHash<int, QByteArray> roleNames() const override;

private:
    Q_DISABLE_COPY(FilesForRoomModel)
    QVector<File> mFiles;
    RocketChatAccount *mRochetChantAccount = nullptr;
};

#endif // FILESMODELFORROOM_H
