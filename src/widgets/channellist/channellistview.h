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

#ifndef CHANNELLISTVIEW_H
#define CHANNELLISTVIEW_H

#include <QListView>

#include "libruqolawidgets_private_export.h"

class RoomFilterProxyModel;

class LIBRUQOLAWIDGETS_TESTS_EXPORT ChannelListView : public QListView
{
    Q_OBJECT
public:
    explicit ChannelListView(QWidget *parent = nullptr);
    ~ChannelListView() override;

    RoomFilterProxyModel *model() const;
    void setModel(QAbstractItemModel *model) override;

    void selectChannelRequested(const QString &channelId);
Q_SIGNALS:
    void channelSelected(const QModelIndex &index);

protected:
    void contextMenuEvent(QContextMenuEvent *event) override;

private:
    void slotClicked(const QModelIndex &index);
    void slotHideChannel(const QModelIndex &index, const QString &roomType);
    void slotLeaveChannel(const QModelIndex &index, const QString &roomType);
    void slotChangeFavorite(const QModelIndex &index, bool isFavorite);
};

#endif // CHANNELLISTVIEW_H
