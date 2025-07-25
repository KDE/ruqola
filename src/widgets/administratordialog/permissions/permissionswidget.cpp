/*
   SPDX-FileCopyrightText: 2021-2025 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "permissionswidget.h"
using namespace Qt::Literals::StringLiterals;

#include "connection.h"
#include "model/permissionsmodel.h"
#include "permissions/permissions.h"
#include "permissions/permissionslistalljob.h"
#include "permissions/permissionupdatejob.h"
#include "permissionseditdialog.h"
#include "permissionstreeview.h"
#include "rocketchataccount.h"
#include "ruqolawidgets_debug.h"
#include <KLineEditEventHandler>
#include <KLocalizedString>
#include <QLineEdit>
#include <QMenu>
#include <QPointer>
#include <QSortFilterProxyModel>
#include <QVBoxLayout>

PermissionsWidget::PermissionsWidget(RocketChatAccount *account, QWidget *parent)
    : QWidget(parent)
    , mTreeView(new PermissionsTreeView(this))
    , mSearchLineWidget(new QLineEdit(this))
    , mAdminPermissionsModel(new PermissionsModel(this))
    , mPermissionFilterProxyModel(new QSortFilterProxyModel(this))
    , mRocketChatAccount(account)
{
    mPermissionFilterProxyModel->setObjectName(u"permissionFilterProxyModel"_s);

    auto mainLayout = new QVBoxLayout(this);
    mainLayout->setObjectName(u"mainLayout"_s);
    mainLayout->setContentsMargins({});
    mainLayout->setSpacing(0);

    mSearchLineWidget->setObjectName(u"mSearchLineWidget"_s);
    mSearchLineWidget->setPlaceholderText(i18nc("@info:placeholder", "Search permissions…"));
    KLineEditEventHandler::catchReturnKey(mSearchLineWidget);
    mSearchLineWidget->setClearButtonEnabled(true);
    mainLayout->addWidget(mSearchLineWidget);
    mTreeView->setObjectName(u"mTreeView"_s);
    mainLayout->addWidget(mTreeView);
    mPermissionFilterProxyModel->setSourceModel(mAdminPermissionsModel);
    mTreeView->setModel(mPermissionFilterProxyModel);
    mTreeView->setColumnHidden(PermissionsModel::Roles, true);
    connect(mTreeView, &QTreeView::customContextMenuRequested, this, &PermissionsWidget::slotCustomContextMenuRequested);
    connect(mSearchLineWidget, &QLineEdit::textChanged, this, &PermissionsWidget::slotFilterTextChanged);
    connect(mTreeView, &QTreeView::doubleClicked, this, &PermissionsWidget::slotModifyDoubleClickRoles);
}

PermissionsWidget::~PermissionsWidget() = default;

void PermissionsWidget::slotFilterTextChanged(const QString &str)
{
    mPermissionFilterProxyModel->setFilterFixedString(str);
}

void PermissionsWidget::initialize()
{
    // First load list of roles.
    mRoleInfo = mRocketChatAccount->roleInfo();

    auto permissionsListAllJob = new RocketChatRestApi::PermissionsListAllJob(this);
    mRocketChatAccount->restApi()->initializeRestApiJob(permissionsListAllJob);
    connect(permissionsListAllJob, &RocketChatRestApi::PermissionsListAllJob::permissionListAllDone, this, &PermissionsWidget::slotPermissionListAllDone);
    if (!permissionsListAllJob->start()) {
        qCDebug(RUQOLAWIDGETS_LOG) << "Impossible to start PermissionsListAllJob";
    }
}

void PermissionsWidget::slotPermissionListAllDone(const QJsonObject &obj)
{
    Permissions p;
    p.parsePermissions(obj, {}, mRoleInfo);
    mAdminPermissionsModel->setPermissions(p);
    // qDebug() << "obj" << obj;
    for (int i : {PermissionsModel::Identifier, PermissionsModel::RolesStr}) {
        mTreeView->resizeColumnToContents(i);
    }
}

void PermissionsWidget::slotCustomContextMenuRequested(const QPoint &pos)
{
    const QModelIndex index = mTreeView->indexAt(pos);
    if (mRocketChatAccount->hasPermission(u"access-permissions"_s)) {
        if (index.isValid()) {
            QMenu menu(this);
            menu.addAction(QIcon::fromTheme(u"document-edit"_s), i18nc("@action", "Modify…"), this, [this, index]() {
                modifyRoles(index);
            });
            menu.exec(mTreeView->viewport()->mapToGlobal(pos));
        }
    }
}

void PermissionsWidget::slotModifyDoubleClickRoles(const QModelIndex &index)
{
    if (index.isValid()) {
        if (mRocketChatAccount->hasPermission(u"access-permissions"_s)) {
            modifyRoles(index);
        }
    }
}

void PermissionsWidget::modifyRoles(const QModelIndex &index)
{
    const QModelIndex modelIndex = mTreeView->model()->index(index.row(), PermissionsModel::Roles);
    const QString identifier = mTreeView->model()->index(index.row(), PermissionsModel::Identifier).data().toString();
    slotEditRoles(modelIndex.data().toStringList(), identifier);
}

void PermissionsWidget::slotEditRoles(const QStringList &roles, const QString &identifier)
{
    QPointer<PermissionsEditDialog> dialog = new PermissionsEditDialog(this, identifier);
    dialog->setRolesInfo(mRoleInfo);
    dialog->setRoles(roles);
    if (dialog->exec()) {
        const QStringList lst = dialog->roles();
        auto permissionsUpdateJob = new RocketChatRestApi::PermissionUpdateJob(this);
        mRocketChatAccount->restApi()->initializeRestApiJob(permissionsUpdateJob);
        QMap<QString, QStringList> mapPermission;
        mapPermission.insert(identifier, lst);
        permissionsUpdateJob->setPermissions(mapPermission);
        connect(permissionsUpdateJob, &RocketChatRestApi::PermissionUpdateJob::permissionUpdateDone, this, &PermissionsWidget::slotPermissionUpdateDone);
        if (!permissionsUpdateJob->start()) {
            qCDebug(RUQOLAWIDGETS_LOG) << "Impossible to start PermissionUpdateJob";
        }
    }
    delete dialog;
}

void PermissionsWidget::slotPermissionUpdateDone(const QJsonObject &obj)
{
    // qDebug() << " obj " << obj;
    Permissions p;
    p.parsePermissions(obj, u"permissions"_s, mRoleInfo);
    mAdminPermissionsModel->setPermissions(p);
}

#include "moc_permissionswidget.cpp"
