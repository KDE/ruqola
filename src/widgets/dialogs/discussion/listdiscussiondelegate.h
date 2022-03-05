/*
   SPDX-FileCopyrightText: 2020-2022 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#pragma once

#include "libruqolawidgets_private_export.h"
#include <QItemDelegate>
class LIBRUQOLAWIDGETS_TESTS_EXPORT ListDiscussionDelegate : public QItemDelegate
{
    Q_OBJECT
public:
    explicit ListDiscussionDelegate(QObject *parent = nullptr);
    ~ListDiscussionDelegate() override;

    void paint(QPainter *painter, const QStyleOptionViewItem &option, const QModelIndex &index) const override;
    bool editorEvent(QEvent *event, QAbstractItemModel *model, const QStyleOptionViewItem &option, const QModelIndex &index) override;
    QSize sizeHint(const QStyleOptionViewItem &option, const QModelIndex &index) const override;

private:
    struct Layout {
        // Attachment name
        QString text;
        qreal textY = 0;

        // TimeStamp
        QString lastMessageTimeText;
        qreal lastMessageTimeY = 0;

        // Number of message
        int numberOfMessages = 0;

        // OpenDiscussionText
        QString openDiscussionText;
        qreal openDiscussionTextY = 0;
        //
        QRect usableRect;
    };
    Q_REQUIRED_RESULT ListDiscussionDelegate::Layout doLayout(const QStyleOptionViewItem &option, const QModelIndex &index) const;
};

