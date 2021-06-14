/*
   Copyright (c) 2021 Laurent Montel <montel@kde.org>

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

#include "misc/searchtreebasewidget.h"

#include "libruqolawidgets_private_export.h"
class DirectoryBaseFilterProxyModel;
class LIBRUQOLAWIDGETS_TESTS_EXPORT AdministratorUsersWidget : public SearchTreeBaseWidget
{
    Q_OBJECT
public:
    explicit AdministratorUsersWidget(QWidget *parent = nullptr);
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
    Q_REQUIRED_RESULT QString displayShowMessageInRoom() const;
    DirectoryBaseFilterProxyModel *mAdminUsersProxyModel = nullptr;
};

