/*
   SPDX-FileCopyrightText: 2020-2021 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: GPL-2.0-or-later
*/

#pragma once

#include "libruqolawidgets_private_export.h"
#include <QItemDelegate>
class LIBRUQOLAWIDGETS_TESTS_EXPORT SearchChannelDelegate : public QItemDelegate
{
    Q_OBJECT
public:
    explicit SearchChannelDelegate(QObject *parent = nullptr);
    ~SearchChannelDelegate() override;
    void paint(QPainter *painter, const QStyleOptionViewItem &option, const QModelIndex &index) const override;
    bool editorEvent(QEvent *event, QAbstractItemModel *model, const QStyleOptionViewItem &option, const QModelIndex &index) override;
    QSize sizeHint(const QStyleOptionViewItem &option, const QModelIndex &index) const override;

Q_SIGNALS:
    void channelSelected(const QModelIndex &index);

private:
    struct Layout {
        QRect joinButtonRect;

        QRect usableRect;

        QString channelName;

        // Select Channel Rect
        QRect selectChannelRect;
    };
    Q_REQUIRED_RESULT SearchChannelDelegate::Layout doLayout(const QStyleOptionViewItem &option, const QModelIndex &index) const;
    const QString mJoinLabel;
};

