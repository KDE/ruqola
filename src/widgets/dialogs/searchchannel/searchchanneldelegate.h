/*
   Copyright (c) 2020 Laurent Montel <montel@kde.org>

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

#ifndef SEARCHCHANNELDELEGATE_H
#define SEARCHCHANNELDELEGATE_H

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

Q_SIGNALS:
    void channelSelected(const QModelIndex &index);

private:
    struct Layout {

        QRect iconChannelRect;

        QRect usableRect;

        QString channelName;

        //Select Channel Rect
        QRect selectChannelRect;
    };
    SearchChannelDelegate::Layout doLayout(const QStyleOptionViewItem &option, const QModelIndex &index) const;
    QIcon mAddChannel;
};

#endif
