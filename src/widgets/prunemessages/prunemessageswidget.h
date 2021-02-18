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

#pragma once

#include "libruqolawidgets_private_export.h"
#include "rooms/roomscleanhistoryjob.h"
#include <QWidget>
class QCheckBox;
class QDateTimeEdit;
class AddUsersWidget;
class QLabel;
class LIBRUQOLAWIDGETS_TESTS_EXPORT PruneMessagesWidget : public QWidget
{
    Q_OBJECT
public:
    explicit PruneMessagesWidget(QWidget *parent = nullptr);
    ~PruneMessagesWidget() override;

    Q_REQUIRED_RESULT RocketChatRestApi::RoomsCleanHistoryJob::CleanHistoryInfo cleanHistoryInfo() const;

    void setRoomName(const QString &roomName);
Q_SIGNALS:
    void updateOkButton(bool b);

private:
    void updateLabelInfo();
    void slotCheckDateTime();
    QString mRoomName;
    QCheckBox *const mInclusive;
    QCheckBox *const mDoNotPrunePinnedMessage;
    QCheckBox *const mDoNotPruneDiscussionMessage;
    QCheckBox *const mDoNotPruneThreads;
    QCheckBox *const mOnlyRemoveAttachedFiles;
    QDateTimeEdit *const mLastestDateTimeEdit;
    QDateTimeEdit *const mOldestDateTimeEdit;
    AddUsersWidget *const mUsers;
    QLabel *const mInfoLabel;
};

