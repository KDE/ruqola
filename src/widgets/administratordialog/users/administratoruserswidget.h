/*
   SPDX-FileCopyrightText: 2021-2022 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#pragma once

#include "misc/searchtreebasewidget.h"

#include "libruqolawidgets_private_export.h"
class LIBRUQOLAWIDGETS_TESTS_EXPORT AdministratorUsersWidget : public SearchTreeBaseWidget
{
    Q_OBJECT
public:
    explicit AdministratorUsersWidget(RocketChatAccount *account, QWidget *parent = nullptr);
    ~AdministratorUsersWidget() override;

protected:
    void updateLabel() override;
    void slotLoadElements(int offset = -1, int count = -1, const QString &searchName = {}) override;
    void slotCustomContextMenuRequested(const QPoint &pos) override;

private:
    void slotTextChanged(const QString &str);
    void slotAddUser();
    void slotModifyUser(const QModelIndex &index);
    void slotRemoveUser(const QModelIndex &index);
    void slotLoadMoreElements();
    void finishSearching();
    void slotSearchCleared();
    void slotSearchRequested(const QString &str);
    void slotActivateUser(const QModelIndex &index, bool activateUser);
    void slotSetUserActiveStatus(const QJsonObject &replyObject, const QModelIndex &modelIndex);
    void slotDeleteUserDone(const QString &userId);
    void slotUserCreateDone(const QJsonObject &obj);
    void slotUserInfoDone(const QJsonObject &obj);
    void slotUserUpdateDone(const QJsonObject &obj);
    void slotModifyDoubleClickUser(const QModelIndex &index);
    void slotResetE2EKey(const QModelIndex &index);
    Q_REQUIRED_RESULT QString displayShowMessageInRoom() const;
    void slotResetTOTPKey(const QModelIndex &index);
};

