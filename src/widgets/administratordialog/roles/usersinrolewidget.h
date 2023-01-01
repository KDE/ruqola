/*
   SPDX-FileCopyrightText: 2021-2023 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#pragma once

#include "misc/searchtreebasewidget.h"

#include "libruqolawidgets_private_export.h"
class LIBRUQOLAWIDGETS_TESTS_EXPORT UsersInRoleWidget : public SearchTreeBaseWidget
{
    Q_OBJECT
public:
    explicit UsersInRoleWidget(RocketChatAccount *account, QWidget *parent = nullptr);
    ~UsersInRoleWidget() override;

    Q_REQUIRED_RESULT const QString &roleId() const;
    void setRoleId(const QString &newRoleId);

    Q_REQUIRED_RESULT const QString &roleName() const;
    void setRoleName(const QString &newRoleName);

protected:
    void updateLabel() override;
    void slotLoadElements(int offset = -1, int count = -1, const QString &searchName = {}) override;
    void slotCustomContextMenuRequested(const QPoint &pos) override;

private:
    void slotTextChanged(const QString &str);
    void slotLoadMoreElements();
    void finishSearching();
    void slotSearchCleared();
    void slotSearchRequested(const QString &str);
    void slotActivateUser(const QModelIndex &index, bool activateUser);
    void slotAddUser();
    void slotRemoveUser(const QModelIndex &index);
    void slotRemoveUsersFromRoleDone(const QJsonObject &replyObject);
    void slotAddUsersToRoleDone(const QJsonObject &replyObject);
    Q_REQUIRED_RESULT QString displayShowMessageInRoom() const;
    QString mRoleId;
    QString mRoleName;
};
