/*
   SPDX-FileCopyrightText: 2020-2026 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#pragma once

#include "libruqolawidgets_private_export.h"
#include <QItemDelegate>
class RocketChatAccount;
class File;
class LIBRUQOLAWIDGETS_TESTS_EXPORT ListAttachmentDelegate : public QItemDelegate
{
    Q_OBJECT
public:
    explicit ListAttachmentDelegate(RocketChatAccount *account, QObject *parent = nullptr);
    ~ListAttachmentDelegate() override;

    void paint(QPainter *painter, const QStyleOptionViewItem &option, const QModelIndex &index) const override;
    [[nodiscard]] bool editorEvent(QEvent *event, QAbstractItemModel *model, const QStyleOptionViewItem &option, const QModelIndex &index) override;
    [[nodiscard]] QSize sizeHint(const QStyleOptionViewItem &option, const QModelIndex &index) const override;

Q_SIGNALS:
    void deleteAttachment(const QByteArray &fileId);
    void showImage(const QByteArray &fileId);

private:
    struct Layout {
        // Icon mimetype
        qreal mimetypeHeight = 0;

        // Attachment name
        QString attachmentName;
        qreal attachmentNameY = 0;

        // Sender
        QString senderText;
        QFont senderFont;
        qreal senderY = 0;

        // TimeStamp
        QString timeStampText;
        qreal timeStampY = 0;
        //
        QRect usableRect;

        // Download Attachment Rect
        QRect downloadAttachmentRect;

        // Delete Attachment Rect
        QRect deleteAttachmentRect;
    };
    [[nodiscard]] LIBRUQOLAWIDGETS_NO_EXPORT ListAttachmentDelegate::Layout doLayout(const QStyleOptionViewItem &option, const QModelIndex &index) const;
    LIBRUQOLAWIDGETS_NO_EXPORT void saveAttachment(const QStyleOptionViewItem &option, const File *file);
    const QIcon mDownloadIcon;
    const QIcon mDeleteIcon;
    RocketChatAccount *const mRocketChatAccount;
};
