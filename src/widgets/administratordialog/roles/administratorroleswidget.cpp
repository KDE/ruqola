/*
   SPDX-FileCopyrightText: 2021-2023 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "administratorroleswidget.h"
#include "connection.h"
#include "misc/lineeditcatchreturnkey.h"
#include "model/adminrolesmodel.h"
#include "rocketchataccount.h"
#include "roleeditdialog.h"
#include "rolestreeview.h"
#include "ruqolawidgets_debug.h"
#include "userinroleeditdialog.h"

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

AdministratorRolesWidget::AdministratorRolesWidget(RocketChatAccount *account, QWidget *parent)
    : QWidget{parent}
    , mTreeView(new RolesTreeView(this))
    , mSearchLineWidget(new QLineEdit(this))
    , mAdminRolesModel(new AdminRolesModel(this))
    , mRoleFilterProxyModel(new QSortFilterProxyModel(this))
    , mRocketChatAccount(account)
{
    auto mainLayout = new QVBoxLayout(this);
    mainLayout->setObjectName(QStringLiteral("mainLayout"));

    mTreeView->setObjectName(QStringLiteral("mTreeView"));
    mSearchLineWidget->setObjectName(QStringLiteral("mSearchLineWidget"));
    mSearchLineWidget->setPlaceholderText(i18n("Search roles..."));
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
    connect(mTreeView, &QTreeView::doubleClicked, this, &AdministratorRolesWidget::slotModifyDoubleClickRoles);
}

AdministratorRolesWidget::~AdministratorRolesWidget() = default;

void AdministratorRolesWidget::slotModifyDoubleClickRoles(const QModelIndex &index)
{
    if (index.isValid()) {
        if (mRocketChatAccount->hasPermission(QStringLiteral("access-permissions"))) {
            modifyRole(index);
        }
    }
}

void AdministratorRolesWidget::initialize()
{
    auto updateRoleList = [this]() {
        mAdminRolesModel->setRoles(mRocketChatAccount->roleInfo());
    };

    connect(mRocketChatAccount, &RocketChatAccount::rolesUpdated, this, updateRoleList);
    updateRoleList();
    for (int i : {AdminRolesModel::AdminRoles::Name}) {
        mTreeView->resizeColumnToContents(i);
    }
}

void AdministratorRolesWidget::slotFilterTextChanged(const QString &str)
{
    mRoleFilterProxyModel->setFilterFixedString(str);
}

void AdministratorRolesWidget::slotCustomContextMenuRequested(const QPoint &pos)
{
    const QModelIndex index = mTreeView->indexAt(pos);

    if (mRocketChatAccount->hasPermission(QStringLiteral("access-permissions"))) { // For delete
        QMenu menu(this);
        menu.addAction(QIcon::fromTheme(QStringLiteral("list-add")), i18n("Add..."), this, &AdministratorRolesWidget::addRole);
        if (index.isValid()) {
            menu.addAction(QIcon::fromTheme(QStringLiteral("document-edit")), i18n("Modify..."), this, [this, index]() {
                const QModelIndex modelIndex = mTreeView->model()->index(index.row(), AdminRolesModel::Identifier);
                modifyRole(modelIndex);
            });
            menu.addSeparator();

            menu.addAction(QIcon::fromTheme(QStringLiteral("document-edit")), i18n("Add Users In Role..."), this, [this, index]() {
                const QModelIndex modelIndex = mTreeView->model()->index(index.row(), AdminRolesModel::Identifier);
                addUserInRole(modelIndex);
            });

            const QModelIndex modelIndex = mTreeView->model()->index(index.row(), AdminRolesModel::Protected);
            if (!modelIndex.data().toBool()) { // Not protected we can delete it.
                menu.addSeparator();
                menu.addAction(QIcon::fromTheme(QStringLiteral("list-remove")), i18n("Remove"), this, [this, index]() {
                    const QModelIndex modelIndex = mTreeView->model()->index(index.row(), AdminRolesModel::Identifier);
                    deleteRole(modelIndex);
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
        auto roleCreateJob = new RocketChatRestApi::RoleCreateJob(this);
        mRocketChatAccount->restApi()->initializeRestApiJob(roleCreateJob);
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
    qCDebug(RUQOLAWIDGETS_LOG) << " AdministratorRolesWidget::slotRoleCreateDone ";
}

void AdministratorRolesWidget::addUserInRole(const QModelIndex &modelIndex)
{
    UserInRoleEditDialog dlg(mRocketChatAccount, this);
    dlg.setRoleId(modelIndex.data().toString());
    const QModelIndex index = mTreeView->model()->index(modelIndex.row(), AdminRolesModel::Name);
    dlg.setRoleName(index.data().toString());
    dlg.exec();
}

void AdministratorRolesWidget::modifyRole(const QModelIndex &modelIndex)
{
    if (!modelIndex.isValid()) {
        return;
    }
    QPointer<RoleEditDialog> dlg = new RoleEditDialog(this);
    RoleEditWidget::RoleEditDialogInfo currentRole;
    QModelIndex index = mTreeView->model()->index(modelIndex.row(), AdminRolesModel::Name);
    currentRole.mName = index.data().toString();

    index = mTreeView->model()->index(modelIndex.row(), AdminRolesModel::Mandatory2Fa);
    currentRole.mTwoFactor = index.data().toBool();

    index = mTreeView->model()->index(modelIndex.row(), AdminRolesModel::Scope);
    currentRole.mScope = index.data().toString();

    index = mTreeView->model()->index(modelIndex.row(), AdminRolesModel::Description);
    currentRole.mDescription = index.data().toString();
    index = mTreeView->model()->index(modelIndex.row(), AdminRolesModel::Protected);
    currentRole.mIsProtected = index.data().toBool();

    dlg->setRoleEditDialogInfo(currentRole);
    if (dlg->exec()) {
        const RoleEditWidget::RoleEditDialogInfo newRoleInfo = dlg->roleEditDialogInfo();
        if (currentRole != newRoleInfo) {
            auto roleUpdateJob = new RocketChatRestApi::RoleUpdateJob(this);
            mRocketChatAccount->restApi()->initializeRestApiJob(roleUpdateJob);
            RocketChatRestApi::RoleUpdateJob::RoleUpdateInfo updateInfo;
            updateInfo.description = newRoleInfo.mDescription;
            updateInfo.name = newRoleInfo.mName;
            updateInfo.mandatory2fa = newRoleInfo.mTwoFactor;
            updateInfo.scope = newRoleInfo.mScope;
            index = mTreeView->model()->index(modelIndex.row(), AdminRolesModel::Identifier);
            updateInfo.identifier = index.data().toString();

            roleUpdateJob->setUpdateRoleInfo(updateInfo);
            connect(roleUpdateJob, &RocketChatRestApi::RoleUpdateJob::updateRoleDone, this, &AdministratorRolesWidget::slotRoleUpdateDone);
            if (!roleUpdateJob->start()) {
                qCDebug(RUQOLAWIDGETS_LOG) << "Impossible to start RoleUpdateJob";
            }
        }
    }
    delete dlg;
}

void AdministratorRolesWidget::slotRoleUpdateDone()
{
    qCDebug(RUQOLAWIDGETS_LOG) << "slotRoleUpdateDone";
}

void AdministratorRolesWidget::deleteRole(const QModelIndex &modelIndex)
{
    QModelIndex index = mTreeView->model()->index(modelIndex.row(), AdminRolesModel::Identifier);
    const QString identifier = index.data().toString();
    if (identifier.isEmpty()) {
        qCDebug(RUQOLAWIDGETS_LOG) << "role identifier is empty! It's a bug";
        return;
    }
    index = mTreeView->model()->index(modelIndex.row(), AdminRolesModel::Name);
    if (KMessageBox::ButtonCode::PrimaryAction
        == KMessageBox::questionTwoActions(this,
                                           i18n("Do you want to remove this role \'%1\'?", index.data().toString()),
                                           i18n("Remove Role"),
                                           KStandardGuiItem::remove(),
                                           KStandardGuiItem::cancel())) {
        auto roleDeleteJob = new RocketChatRestApi::RoleDeleteJob(this);
        mRocketChatAccount->restApi()->initializeRestApiJob(roleDeleteJob);
        roleDeleteJob->setRoleId(identifier);
        connect(roleDeleteJob, &RocketChatRestApi::RoleDeleteJob::deleteRoleDone, this, &AdministratorRolesWidget::slotRoleDeleteDone);
        if (!roleDeleteJob->start()) {
            qCDebug(RUQOLAWIDGETS_LOG) << "Impossible to start RoleUpdateJob";
        }
    }
}

void AdministratorRolesWidget::slotRoleDeleteDone()
{
    qCDebug(RUQOLAWIDGETS_LOG) << "slotRoleDeleteDone";
}
