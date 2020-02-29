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
    enum class Mode {
        ThreadEditing,
        Editing,
        Viewing, /*when we show list of message as mentions, search etc.*/
    };

    explicit MessageListView(MessageListView::Mode mode, QWidget *parent = nullptr);
    ~MessageListView() override;

    void setChannelSelected(const QString &roomId);

    void setModel(QAbstractItemModel *newModel) override;

    void handleKeyPressEvent(QKeyEvent *ev);

    Q_REQUIRED_RESULT MessageListView::Mode mode() const;

    void setRoomId(const QString &roomID);

    void goToMessage(const QString &messageId);

protected:
    void resizeEvent(QResizeEvent *ev) override;
    void contextMenuEvent(QContextMenuEvent *event) override;

Q_SIGNALS:
    void modelChanged();
    void editMessageRequested(const QString &messageId, const QString &text);
    void createNewDiscussion(const QString &messageId, const QString &originalMessage);
    void loadHistoryRequested();
    void goToMessageRequested(const QString &messageId);

private:
    void checkIfAtBottom();
    void maybeScrollToBottom();
    void slotEditMessage(const QModelIndex &index);
    void slotDeleteMessage(const QModelIndex &index);
    void slotReportMessage(const QModelIndex &index);
    void slotSetAsFavorite(const QModelIndex &index, bool isStarred);
    void slotSetPinnedMessage(const QModelIndex &index, bool isPinned);
    void slotStartDiscussion(const QModelIndex &index);
    void slotCopyText(const QModelIndex &index);
    void slotTranslateMessage(const QModelIndex &index, bool checked);
    void createSeparator(QMenu &menu);
    void slotVerticalScrollbarChanged(int value);

private:
    QString mRoomId;
    MessageListView::Mode mMode = MessageListView::Mode::Editing;
    bool mAtBottom = true;
};

#endif // MESSAGELISTVIEW_H
