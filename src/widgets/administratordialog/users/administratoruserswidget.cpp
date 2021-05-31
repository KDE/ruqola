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

#include "administratoruserswidget.h"
#include "misc/lineeditcatchreturnkey.h"
#include "model/adminusersfilterproxymodel.h"
#include "model/adminusersmodel.h"
#include "restapirequest.h"
#include "rocketchataccount.h"
#include "ruqola.h"
#include "ruqolawidgets_debug.h"
#include "users/userslistjob.h"
#include <KLocalizedString>
#include <QHeaderView>
#include <QLineEdit>
#include <QMenu>
#include <QTreeView>
#include <QVBoxLayout>

AdministratorUsersWidget::AdministratorUsersWidget(QWidget *parent)
    : QWidget(parent)
    , mSearchLineEdit(new QLineEdit(this))
    , mResultTreeView(new QTreeView(this))
    , mAdminUsersModel(new AdminUsersModel(this))
{
    auto mainLayout = new QVBoxLayout(this);
    mainLayout->setObjectName(QStringLiteral("mainLayout"));

    mSearchLineEdit->setObjectName(QStringLiteral("mSearchLineEdit"));
    mSearchLineEdit->setPlaceholderText(i18n("Search users..."));
    mSearchLineEdit->setClearButtonEnabled(true);
    new LineEditCatchReturnKey(mSearchLineEdit, this);
    connect(mSearchLineEdit, &QLineEdit::textChanged, this, &AdministratorUsersWidget::slotTextChanged);
    mainLayout->addWidget(mSearchLineEdit);

    mResultTreeView->setSortingEnabled(true);
    mResultTreeView->setObjectName(QStringLiteral("mResultTreeWidget"));
    mResultTreeView->setSelectionBehavior(QAbstractItemView::SelectRows);
    mResultTreeView->setSelectionMode(QAbstractItemView::SelectionMode::SingleSelection);
    mResultTreeView->setContextMenuPolicy(Qt::CustomContextMenu);
    mResultTreeView->setRootIsDecorated(false);
    connect(mResultTreeView, &QTreeView::customContextMenuRequested, this, &AdministratorUsersWidget::slotCustomContextMenuRequested);

    mainLayout->addWidget(mResultTreeView);

    mAdminUsersModel->setObjectName(QStringLiteral("mAdminUsersModel"));

    mAdminUsersProxyModel = new AdminUsersFilterProxyModel(mAdminUsersModel, this);
    mAdminUsersProxyModel->setObjectName(QStringLiteral("mAdminUsersProxyModel"));

    mResultTreeView->setModel(mAdminUsersProxyModel);
}

AdministratorUsersWidget::~AdministratorUsersWidget()
{
}

void AdministratorUsersWidget::slotTextChanged(const QString &str)
{
    mAdminUsersProxyModel->setFilterString(str);
}

void AdministratorUsersWidget::initialize()
{
    auto *rcAccount = Ruqola::self()->rocketChatAccount();
    auto adminUsersJob = new RocketChatRestApi::UsersListJob(this);
    rcAccount->restApi()->initializeRestApiJob(adminUsersJob);
    connect(adminUsersJob, &RocketChatRestApi::UsersListJob::userListDone, this, &AdministratorUsersWidget::slotAdminUserDone);
    if (!adminUsersJob->start()) {
        qCDebug(RUQOLAWIDGETS_LOG) << "Impossible to start AdminRoomsJob";
    }
}

void AdministratorUsersWidget::slotAdminUserDone(const QJsonObject &obj)
{
    mAdminUsersModel->parseElements(obj);
}

void AdministratorUsersWidget::slotAddUser()
{
}

void AdministratorUsersWidget::slotModifyUser(const QModelIndex &index)
{
}

void AdministratorUsersWidget::slotRemoveUser(const QModelIndex &index)
{
}

void AdministratorUsersWidget::slotCustomContextMenuRequested(const QPoint &pos)
{
    QMenu menu(this);
    menu.addAction(QIcon::fromTheme(QStringLiteral("list-add")), i18n("Add..."), this, &AdministratorUsersWidget::slotAddUser);
    const QModelIndex index = mResultTreeView->indexAt(pos);
    if (index.isValid()) {
        menu.addAction(QIcon::fromTheme(QStringLiteral("document-edit")), i18n("Modify..."), this, [this, index]() {
            slotModifyUser(index);
        });
        menu.addSeparator();
        menu.addAction(QIcon::fromTheme(QStringLiteral("list-remove")), i18n("Remove"), this, [this, index]() {
            slotRemoveUser(index);
        });
    }
    menu.exec(mResultTreeView->viewport()->mapToGlobal(pos));
}
