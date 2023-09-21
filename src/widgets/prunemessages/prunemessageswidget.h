/*
   SPDX-FileCopyrightText: 2020-2023 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#pragma once

#include "libruqolawidgets_private_export.h"
#include "rooms/roomscleanhistoryjob.h"
#include <QWidget>
class QCheckBox;
class QDateTimeEdit;
class AddUsersWidget;
class QLabel;
class RocketChatAccount;
class LIBRUQOLAWIDGETS_TESTS_EXPORT PruneMessagesWidget : public QWidget
{
    Q_OBJECT
public:
    explicit PruneMessagesWidget(RocketChatAccount *account, QWidget *parent = nullptr);
    ~PruneMessagesWidget() override;

    [[nodiscard]] RocketChatRestApi::RoomsCleanHistoryJob::CleanHistoryInfo cleanHistoryInfo() const;

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
