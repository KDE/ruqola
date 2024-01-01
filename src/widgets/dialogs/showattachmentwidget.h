/*
   SPDX-FileCopyrightText: 2020-2024 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#pragma once

#include "libruqolawidgets_private_export.h"
#include <QWidget>
class QLineEdit;
class QLabel;
class QListView;
class FilesForRoomFilterProxyModel;
class ShowAttachmentComboBox;
class RocketChatAccount;
class LIBRUQOLAWIDGETS_TESTS_EXPORT ShowAttachmentWidget : public QWidget
{
    Q_OBJECT
public:
    explicit ShowAttachmentWidget(RocketChatAccount *account, QWidget *parent = nullptr);
    ~ShowAttachmentWidget() override;
    void setModel(FilesForRoomFilterProxyModel *model);

Q_SIGNALS:
    void loadMoreFileAttachment();
    void deleteAttachment(const QString &fileId);

private:
    void slotSearchMessageTextChanged(const QString &str);
    void slotChangeAttachmentType(int index);
    void updateLabel();
    [[nodiscard]] QString displayShowMessageInRoom() const;
    QLineEdit *const mSearchAttachmentFileLineEdit;
    ShowAttachmentComboBox *const mAttachmentCombobox;
    QLabel *const mInfo;
    QListView *const mListAttachment;
    FilesForRoomFilterProxyModel *mModel = nullptr;
};
