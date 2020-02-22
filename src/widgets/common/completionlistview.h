/*
   Copyright (c) 2020 David Faure <faure@kde.org>

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

#ifndef COMPLETIONLISTVIEW_H
#define COMPLETIONLISTVIEW_H

#include <QListView>

/**
 * Completion popup for nicks, channels, etc.
 * QCompleter does the filtering itself... so we need to implement our own popup, instead.
 */
class CompletionListView : public QListView
{
    Q_OBJECT
public:
    CompletionListView();

    void setTextWidget(QWidget *textWidget);

    void setModel(QAbstractItemModel *model) override;

    void slotCompletionAvailable();

Q_SIGNALS:
    void complete(const QModelIndex &currentIndex);

protected:
    void keyPressEvent(QKeyEvent *event) override;

private:

    QWidget *mTextWidget = nullptr;
};

#endif // COMPLETIONLISTVIEW_H
