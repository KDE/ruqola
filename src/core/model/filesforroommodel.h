/*
   Copyright (c) 2018-2021 Laurent Montel <montel@kde.org>

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

#include "attachments/file.h"
#include "attachments/fileattachments.h"
#include "libruqolacore_export.h"
#include <QAbstractListModel>
class RocketChatAccount;
class LIBRUQOLACORE_EXPORT FilesForRoomModel : public QAbstractListModel
{
    Q_OBJECT
public:
    enum UserRoles {
        FileName = Qt::UserRole + 1,
        FilePointer,
        UserId,
        Description,
        Url,
        MimeType,
        CanBeDeleted,
        FileId,
        TimeStamp,
        UserName,
        SortByTimeStamp,
        Complete,
        TypeGroup,
    };
    Q_ENUM(UserRoles)

    explicit FilesForRoomModel(RocketChatAccount *account = nullptr, QObject *parent = nullptr);
    ~FilesForRoomModel() override;

    Q_REQUIRED_RESULT int rowCount(const QModelIndex &parent = QModelIndex()) const override;
    Q_REQUIRED_RESULT QVariant data(const QModelIndex &index, int role) const override;

    void setFiles(const QVector<File> &files);

    FileAttachments *fileAttachments() const;

    Q_REQUIRED_RESULT QString roomId() const;
    void setRoomId(const QString &roomId);

    void parseFileAttachments(const QJsonObject &fileAttachmentsObj, const QString &roomId);
    void addMoreFileAttachments(const QJsonObject &fileAttachmentsObj);
    void initialize();
    Q_REQUIRED_RESULT int total() const;

    Q_REQUIRED_RESULT bool hasFullList() const;
    void setHasFullList(bool state);

    Q_REQUIRED_RESULT bool loadMoreFilesInProgress() const;
    void setLoadMoreFilesInProgress(bool loadMoreFilesInProgress);

Q_SIGNALS:
    void hasFullListChanged();
    void totalChanged();
    void loadingInProgressChanged();

private:
    Q_DISABLE_COPY(FilesForRoomModel)
    void checkFullList();
    QString mRoomId;
    bool mHasFullList = false;
    bool mLoadMoreFilesInProgress = false;
    FileAttachments *const mFileAttachments;
    RocketChatAccount *const mRochetChantAccount;
};

