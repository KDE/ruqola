/*
   Copyright (c) 2020-2021 Laurent Montel <montel@kde.org>

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

#ifndef PRUNEMESSAGESWIDGET_H
#define PRUNEMESSAGESWIDGET_H

#include <QWidget>
#include "rooms/roomscleanhistoryjob.h"
#include "libruqolawidgets_private_export.h"
class QCheckBox;
class QDateTimeEdit;
class AddUsersWidget;
class LIBRUQOLAWIDGETS_TESTS_EXPORT PruneMessagesWidget : public QWidget
{
    Q_OBJECT
public:
    explicit PruneMessagesWidget(QWidget *parent = nullptr);
    ~PruneMessagesWidget() override;

    Q_REQUIRED_RESULT RocketChatRestApi::RoomsCleanHistoryJob::CleanHistoryInfo cleanHistoryInfo() const;

Q_SIGNALS:
    void updateOkButton(bool b);

private:
    void slotCheckDateTime();
    QCheckBox *mInclusive = nullptr;
    QCheckBox *mDoNotPrunePinnedMessage = nullptr;
    QCheckBox *mDoNotPruneDiscussionMessage = nullptr;
    QCheckBox *mDoNotPruneThreads = nullptr;
    QCheckBox *mOnlyRemoveAttachedFiles = nullptr;
    QDateTimeEdit *mLastestDateTimeEdit = nullptr;
    QDateTimeEdit *mOldestDateTimeEdit = nullptr;
    AddUsersWidget *mUsers = nullptr;
};

#endif // PRUNEMESSAGESWIDGET_H
