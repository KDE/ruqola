/*
   SPDX-FileCopyrightText: 2021-2024 Laurent Montel <montel@kde.org>

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

    [[nodiscard]] const QString &roleId() const;
    void setRoleId(const QString &newRoleId);

    [[nodiscard]] const QString &roleName() const;
    void setRoleName(const QString &newRoleName);

protected:
    void updateLabel() override;
    void slotLoadElements(int offset = -1, int count = -1, const QString &searchName = {}) override;
    void slotCustomContextMenuRequested(const QPoint &pos) override;

private:
    LIBRUQOLAWIDGETS_NO_EXPORT void slotTextChanged(const QString &str);
    LIBRUQOLAWIDGETS_NO_EXPORT void slotLoadMoreElements();
    LIBRUQOLAWIDGETS_NO_EXPORT void finishSearching();
    LIBRUQOLAWIDGETS_NO_EXPORT void slotSearchCleared();
    LIBRUQOLAWIDGETS_NO_EXPORT void slotSearchRequested(const QString &str);
    LIBRUQOLAWIDGETS_NO_EXPORT void slotActivateUser(const QModelIndex &index, bool activateUser);
    LIBRUQOLAWIDGETS_NO_EXPORT void slotAddUser();
    LIBRUQOLAWIDGETS_NO_EXPORT void slotRemoveUser(const QModelIndex &index);
    LIBRUQOLAWIDGETS_NO_EXPORT void slotRemoveUsersFromRoleDone(const QJsonObject &replyObject);
    LIBRUQOLAWIDGETS_NO_EXPORT void slotAddUsersToRoleDone(const QJsonObject &replyObject);
    [[nodiscard]] LIBRUQOLAWIDGETS_NO_EXPORT QString displayShowMessageInRoom() const;
    QString mRoleId;
    QString mRoleName;
};
