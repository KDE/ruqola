/*
   SPDX-FileCopyrightText: 2020-2023 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#pragma once

#include "libruqolawidgets_private_export.h"
#include "room.h"
#include <QDialog>
class ShowAttachmentWidget;
class FilesForRoomFilterProxyModel;
class LIBRUQOLAWIDGETS_TESTS_EXPORT ShowAttachmentDialog : public QDialog
{
    Q_OBJECT
public:
    explicit ShowAttachmentDialog(RocketChatAccount *account, QWidget *parent = nullptr);
    ~ShowAttachmentDialog() override;

    void setModel(FilesForRoomFilterProxyModel *model);

    void setRoomId(const QString &roomId);
    Q_REQUIRED_RESULT QString roomId() const;

    Q_REQUIRED_RESULT Room::RoomType roomType() const;
    void setRoomType(Room::RoomType roomType);

private:
    void readConfig();
    void writeConfig();
    void slotLoadMoreAttachment();
    void slotDeleteAttachment(const QString &fileId);
    QString mRoomId;
    Room::RoomType mRoomType = Room::RoomType::Unknown;
    ShowAttachmentWidget *const mShowAttachmentWidget;
    RocketChatAccount *const mRocketChatAccount;
};
