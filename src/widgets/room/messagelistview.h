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

#ifndef MESSAGELISTVIEW_H
#define MESSAGELISTVIEW_H

#include <QListView>
#include "libruqolawidgets_private_export.h"

class LIBRUQOLAWIDGETS_TESTS_EXPORT MessageListView : public QListView
{
    Q_OBJECT
public:
    explicit MessageListView(QWidget *parent = nullptr);
    ~MessageListView();

    void setChannelSelected(const QString &roomId);

    void setModel(QAbstractItemModel *newModel) override;

protected:
    void resizeEvent(QResizeEvent *ev) override;
    void keyPressEvent(QKeyEvent *ev) override;
    void contextMenuEvent(QContextMenuEvent *event) override;

Q_SIGNALS:
    void modelChanged();
    void keyPressed(QKeyEvent *ev);
    void editMessageRequested(const QString &messageId, const QString &text);

private:
    void checkIfAtBottom();
    void maybeScrollToBottom();
    void slotEditMessage(const QModelIndex &index);
    void slotDeleteMessage(const QModelIndex &index);
    void slotReportMessage(const QModelIndex &index);
    void slotSetAsFavorite(const QModelIndex &index, bool isStarred);

private:
    bool mAtBottom = true;
    QString mRoomID;
};

#endif // MESSAGELISTVIEW_H
