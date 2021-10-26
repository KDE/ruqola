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

#include "administratorroleswidget.h"
#include "connection.h"
#include "misc/lineeditcatchreturnkey.h"
#include "model/adminrolesmodel.h"
#include "rocketchataccount.h"
#include "roleeditdialog.h"
#include "rolestreeview.h"
#include "ruqola.h"
#include "ruqolawidgets_debug.h"

#include "role/rolecreatejob.h"
#include "role/roledeletejob.h"
#include "role/roleupdatejob.h"

#include <KLocalizedString>

#include <KMessageBox>
#include <QHeaderView>
#include <QLineEdit>
#include <QMenu>
#include <QPointer>
#include <QSortFilterProxyModel>
#include <QVBoxLayout>

AdministratorRolesWidget::AdministratorRolesWidget(QWidget *parent)
    : QWidget{parent}
    , mTreeView(new RolesTreeView(this))
    , mSearchLineWidget(new QLineEdit(this))
    , mAdminRolesModel(new AdminRolesModel(this))
    , mRoleFilterProxyModel(new QSortFilterProxyModel(this))
{
    auto mainLayout = new QVBoxLayout(this);
    mainLayout->setObjectName(QStringLiteral("mainLayout"));
    mainLayout->setContentsMargins({});

    mTreeView->setObjectName(QStringLiteral("mTreeView"));
    mSearchLineWidget->setObjectName(QStringLiteral("mSearchLineWidget"));
    mSearchLineWidget->setPlaceholderText(i18n("Search Roles..."));
    mSearchLineWidget->setClearButtonEnabled(true);
    new LineEditCatchReturnKey(mSearchLineWidget, this);

    mainLayout->addWidget(mSearchLineWidget);
    mainLayout->addWidget(mTreeView);

    mRoleFilterProxyModel->setObjectName(QStringLiteral("mRoleFilterProxyModel"));
    mRoleFilterProxyModel->setSourceModel(mAdminRolesModel);
    mTreeView->setModel(mRoleFilterProxyModel);

    mTreeView->header()->hideSection(AdminRolesModel::Identifier);

    connect(mSearchLineWidget, &QLineEdit::textChanged, this, &AdministratorRolesWidget::slotFilterTextChanged);
    connect(mTreeView, &QTreeView::customContextMenuRequested, this, &AdministratorRolesWidget::slotCustomContextMenuRequested);
}

AdministratorRolesWidget::~AdministratorRolesWidget()
{
}

void AdministratorRolesWidget::initialize()
{
    auto *rcAccount = Ruqola::self()->rocketChatAccount();
    auto updateRoleList = [rcAccount, this]() {
        mAdminRolesModel->setRoles(rcAccount->roleInfo());
    };

    connect(rcAccount, &RocketChatAccount::rolesUpdated, this, updateRoleList);
    updateRoleList();
}

void AdministratorRolesWidget::slotFilterTextChanged(const QString &str)
{
    mRoleFilterProxyModel->setFilterFixedString(str);
}

void AdministratorRolesWidget::slotCustomContextMenuRequested(const QPoint &pos)
{
    const QModelIndex index = mTreeView->indexAt(pos);
    auto *rcAccount = Ruqola::self()->rocketChatAccount();

    if (rcAccount->hasPermission(QStringLiteral("access-permissions"))) { //For delete
        QMenu menu(this);
        menu.addAction(QIcon::fromTheme(QStringLiteral("list-add")), i18n("Add..."), this, &AdministratorRolesWidget::addRole);
        if (index.isValid()) {
            menu.addAction(QIcon::fromTheme(QStringLiteral("document-edit")), i18n("Modify..."), this, [this, index]() {
                const QModelIndex modelIndex = mTreeView->model()->index(index.row(), AdminRolesModel::Identifier);
                // TODO verify if it's the correct modelinfo
                modifyRole(modelIndex);
            });
            const QModelIndex modelIndex = mTreeView->model()->index(index.row(), AdminRolesModel::Protected);
            if (!modelIndex.data().toBool()) { // Not protected we can delete it.
                menu.addAction(QIcon::fromTheme(QStringLiteral("list-remove")), i18n("Remove"), this, [this, index]() {
                    const QModelIndex modelIndex = mTreeView->model()->index(index.row(), AdminRolesModel::Identifier);
                    deleteRole(modelIndex.data().toString());
                });
            }
        }
        menu.exec(mTreeView->viewport()->mapToGlobal(pos));
    }
}

void AdministratorRolesWidget::addRole()
{
    QPointer<RoleEditDialog> dlg = new RoleEditDialog(this);
    if (dlg->exec()) {
        const RoleEditWidget::RoleEditDialogInfo info = dlg->roleEditDialogInfo();
        auto *rcAccount = Ruqola::self()->rocketChatAccount();
        auto roleCreateJob = new RocketChatRestApi::RoleCreateJob(this);
        rcAccount->restApi()->initializeRestApiJob(roleCreateJob);
        RocketChatRestApi::RoleCreateJob::RoleCreateInfo createInfo;
        createInfo.description = info.mDescription;
        createInfo.name = info.mName;
        createInfo.mandatory2fa = info.mTwoFactor;
        createInfo.scope = info.mScope;

        roleCreateJob->setCreateRoleInfo(createInfo);
        connect(roleCreateJob, &RocketChatRestApi::RoleCreateJob::createRoleDone, this, &AdministratorRolesWidget::slotRoleCreateDone);
        if (!roleCreateJob->start()) {
            qCDebug(RUQOLAWIDGETS_LOG) << "Impossible to start RoleCreateJob";
        }
    }
    delete dlg;
}

void AdministratorRolesWidget::slotRoleCreateDone()
{
    qDebug() << " AdministratorRolesWidget::slotRoleCreateDone ";
    // TODO
}

void AdministratorRolesWidget::modifyRole(const QModelIndex &modelIndex)
{
    QPointer<RoleEditDialog> dlg = new RoleEditDialog(this);
    RocketChatRestApi::RoleUpdateJob::RoleUpdateInfo updateInfo;
    // TODO
    if (dlg->exec()) {
        const RoleEditWidget::RoleEditDialogInfo info = dlg->roleEditDialogInfo();
        auto *rcAccount = Ruqola::self()->rocketChatAccount();
        auto roleUpdateJob = new RocketChatRestApi::RoleUpdateJob(this);
        rcAccount->restApi()->initializeRestApiJob(roleUpdateJob);
        updateInfo.description = info.mDescription;
        updateInfo.name = info.mName;
        updateInfo.mandatory2fa = info.mTwoFactor;
        updateInfo.scope = info.mScope;
        // TODO updateInfo.identifier =

        roleUpdateJob->setUpdateRoleInfo(updateInfo);
        connect(roleUpdateJob, &RocketChatRestApi::RoleUpdateJob::updateRoleDone, this, &AdministratorRolesWidget::slotRoleUpdateDone);
        if (!roleUpdateJob->start()) {
            qCDebug(RUQOLAWIDGETS_LOG) << "Impossible to start RoleUpdateJob";
        }
    }
    delete dlg;
}

void AdministratorRolesWidget::slotRoleUpdateDone()
{
    // TODO
}

void AdministratorRolesWidget::deleteRole(const QString &identifier)
{
    if (identifier.isEmpty()) {
        qCDebug(RUQOLAWIDGETS_LOG) << "role identifier is empty! It's a bug";
        return;
    }
    if (KMessageBox::questionYesNo(this, i18n("Remove role"), i18n("Do you want to remove this role?")) == KMessageBox::Yes) {
        auto *rcAccount = Ruqola::self()->rocketChatAccount();
        auto roleDeleteJob = new RocketChatRestApi::RoleDeleteJob(this);
        rcAccount->restApi()->initializeRestApiJob(roleDeleteJob);
        roleDeleteJob->setRoleId(identifier);
        connect(roleDeleteJob, &RocketChatRestApi::RoleDeleteJob::deleteRoleDone, this, &AdministratorRolesWidget::slotRoleDeleteDone);
        if (!roleDeleteJob->start()) {
            qCDebug(RUQOLAWIDGETS_LOG) << "Impossible to start RoleUpdateJob";
        }
    }
}

void AdministratorRolesWidget::slotRoleDeleteDone()
{
    // TODO
}
