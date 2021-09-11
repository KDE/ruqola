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

#include "permissionswidget.h"
#include "connection.h"
#include "model/adminpermissionsmodel.h"
#include "permissions.h"
#include "permissions/permissionslistalljob.h"
#include "permissions/permissionupdatejob.h"
#include "permissionseditdialog.h"
#include "permissionstreeview.h"
#include "rocketchataccount.h"
#include "ruqola.h"
#include "ruqolawidgets_debug.h"
#include <KLocalizedString>
#include <QHeaderView>
#include <QLineEdit>
#include <QMenu>
#include <QPointer>
#include <QSortFilterProxyModel>
#include <QVBoxLayout>

PermissionsWidget::PermissionsWidget(QWidget *parent)
    : QWidget(parent)
    , mTreeView(new PermissionsTreeView(this))
    , mSearchLineWidget(new QLineEdit(this))
    , mAdminPermissionsModel(new AdminPermissionsModel(this))
    , mPermissionFilterProxyModel(new QSortFilterProxyModel(this))
{
    mPermissionFilterProxyModel->setObjectName(QStringLiteral("permissionFilterProxyModel"));

    auto mainLayout = new QVBoxLayout(this);
    mainLayout->setObjectName(QStringLiteral("mainLayout"));
    mainLayout->setContentsMargins({});

    mSearchLineWidget->setObjectName(QStringLiteral("mSearchLineWidget"));
    mainLayout->addWidget(mSearchLineWidget);
    mTreeView->setObjectName(QStringLiteral("mTreeView"));
    mainLayout->addWidget(mTreeView);
    mPermissionFilterProxyModel->setSourceModel(mAdminPermissionsModel);
    mTreeView->setModel(mPermissionFilterProxyModel);
    connect(mTreeView, &QTreeView::customContextMenuRequested, this, &PermissionsWidget::slotCustomContextMenuRequested);
}

PermissionsWidget::~PermissionsWidget()
{
}

void PermissionsWidget::initialize()
{
    auto *rcAccount = Ruqola::self()->rocketChatAccount();
    auto permissionsListAllJob = new RocketChatRestApi::PermissionsListAllJob(this);
    rcAccount->restApi()->initializeRestApiJob(permissionsListAllJob);
    connect(permissionsListAllJob, &RocketChatRestApi::PermissionsListAllJob::permissionListAllDone, this, &PermissionsWidget::slotPermissionListAllDone);
    if (!permissionsListAllJob->start()) {
        qCDebug(RUQOLAWIDGETS_LOG) << "Impossible to start PermissionsListAllJob";
    }
}

void PermissionsWidget::slotPermissionListAllDone(const QJsonObject &obj)
{
    Permissions p;
    p.parsePermissions(obj);
    mAdminPermissionsModel->setPermissions(p);
    // qDebug() << "obj" << obj;
}

void PermissionsWidget::slotCustomContextMenuRequested(const QPoint &pos)
{
    const QModelIndex index = mTreeView->indexAt(pos);
    if (index.isValid()) {
        QMenu menu(this);
        menu.addAction(QIcon::fromTheme(QStringLiteral("document-edit")), i18n("Modify..."), this, [this, index]() {
            const QModelIndex modelIndex = mTreeView->model()->index(index.row(), AdminPermissionsModel::Roles);
            const QString identifier = mTreeView->model()->index(index.row(), AdminPermissionsModel::Identifier).data().toString();
            slotEditRoles(modelIndex.data().toString().split(QLatin1Char(',')), identifier);
        });
        menu.exec(mTreeView->viewport()->mapToGlobal(pos));
    }
}

void PermissionsWidget::slotEditRoles(const QStringList &roles, const QString &identifier)
{
    QPointer<PermissionsEditDialog> dialog = new PermissionsEditDialog(this, identifier);
    dialog->setRoles(roles);
    if (dialog->exec()) {
        const QStringList lst = dialog->roles();
        auto *rcAccount = Ruqola::self()->rocketChatAccount();
        auto permissionsUpdateJob = new RocketChatRestApi::PermissionUpdateJob(this);
        rcAccount->restApi()->initializeRestApiJob(permissionsUpdateJob);
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
    qDebug() << " obj " << obj;
    Permissions p;
    p.parsePermissions(obj, QStringLiteral("permissions"));
    mAdminPermissionsModel->setPermissions(p);
}
