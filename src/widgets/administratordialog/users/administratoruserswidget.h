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

#ifndef ADMINISTRATORUSERSWIDGET_H
#define ADMINISTRATORUSERSWIDGET_H

#include <QWidget>

#include "libruqolawidgets_private_export.h"
class QLineEdit;
class QTableView;
class AdminUsersModel;
class AdminUsersFilterProxyModel;
class LIBRUQOLAWIDGETS_TESTS_EXPORT AdministratorUsersWidget : public QWidget
{
    Q_OBJECT
public:
    explicit AdministratorUsersWidget(QWidget *parent = nullptr);
    ~AdministratorUsersWidget() override;

private:
    void slotTextChanged(const QString &str);
    void initialize();
    void slotAdminUserDone(const QJsonObject &obj);
    void slotCustomContextMenuRequested(const QPoint &pos);
    void slotAddUser();
    void slotModifyUser(const QModelIndex &index);
    void slotRemoveUser(const QModelIndex &index);
    QLineEdit *const mSearchLineEdit;
    QTableView *const mResultTreeWidget;
    AdminUsersModel *mAdminUsersModel = nullptr;
    AdminUsersFilterProxyModel *mAdminUsersProxyModel = nullptr;
};

#endif // ADMINISTRATORUSERSWIDGET_H
