/*
   SPDX-FileCopyrightText: 2021-2024 Laurent Montel <montel@kde.org>

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
    LIBRUQOLAWIDGETS_NO_EXPORT void slotTextChanged(const QString &str);
    LIBRUQOLAWIDGETS_NO_EXPORT void slotAddUser();
    LIBRUQOLAWIDGETS_NO_EXPORT void slotModifyUser(const QModelIndex &index);
    LIBRUQOLAWIDGETS_NO_EXPORT void slotRemoveUser(const QModelIndex &index);
    LIBRUQOLAWIDGETS_NO_EXPORT void slotLoadMoreElements();
    LIBRUQOLAWIDGETS_NO_EXPORT void finishSearching();
    LIBRUQOLAWIDGETS_NO_EXPORT void slotSearchCleared();
    LIBRUQOLAWIDGETS_NO_EXPORT void slotSearchRequested(const QString &str);
    LIBRUQOLAWIDGETS_NO_EXPORT void slotActivateUser(const QModelIndex &index, bool activateUser);
    LIBRUQOLAWIDGETS_NO_EXPORT void slotSetUserActiveStatus(const QJsonObject &replyObject, const QModelIndex &modelIndex);
    LIBRUQOLAWIDGETS_NO_EXPORT void slotDeleteUserDone(const QString &userId);
    LIBRUQOLAWIDGETS_NO_EXPORT void slotUserCreateDone(const QJsonObject &obj);
    LIBRUQOLAWIDGETS_NO_EXPORT void slotUserInfoDone(const QJsonObject &obj);
    LIBRUQOLAWIDGETS_NO_EXPORT void slotUserUpdateDone(const QJsonObject &obj);
    LIBRUQOLAWIDGETS_NO_EXPORT void slotModifyDoubleClickUser(const QModelIndex &index);
    LIBRUQOLAWIDGETS_NO_EXPORT void slotResetE2EKey(const QModelIndex &index);
    LIBRUQOLAWIDGETS_NO_EXPORT void slotResetTOTPKey(const QModelIndex &index);
    LIBRUQOLAWIDGETS_NO_EXPORT void slotChangeAdmin(const QModelIndex &index, bool adminStatus);
    LIBRUQOLAWIDGETS_NO_EXPORT void slotInviteUsers();
    [[nodiscard]] LIBRUQOLAWIDGETS_NO_EXPORT QString displayShowMessageInRoom() const;
};
