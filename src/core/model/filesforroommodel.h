/*
   SPDX-FileCopyrightText: 2018-2023 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
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
    LIBRUQOLACORE_NO_EXPORT void checkFullList();
    QString mRoomId;
    bool mHasFullList = false;
    bool mLoadMoreFilesInProgress = false;
    FileAttachments *const mFileAttachments;
    RocketChatAccount *const mRochetChantAccount;
};
