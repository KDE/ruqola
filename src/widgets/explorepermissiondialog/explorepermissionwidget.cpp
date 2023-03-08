/*
   SPDX-FileCopyrightText: 2023 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "explorepermissionwidget.h"
#include "misc/lineeditcatchreturnkey.h"
#include "model/permissionsmodel.h"
#include <KLocalizedString>
#include <QHeaderView>
#include <QLineEdit>
#include <QSortFilterProxyModel>
#include <QTreeView>
#include <QVBoxLayout>

ExplorePermissionWidget::ExplorePermissionWidget(QWidget *parent)
    : QWidget{parent}
    , mTreeView(new QTreeView(this))
    , mSearchLineWidget(new QLineEdit(this))
    , mAdminPermissionsModel(new PermissionsModel(this))
    , mPermissionFilterProxyModel(new QSortFilterProxyModel(this))
{
    mPermissionFilterProxyModel->setObjectName(QStringLiteral("permissionFilterProxyModel"));

    mTreeView->setRootIsDecorated(false);
    mTreeView->setSortingEnabled(true);
    mTreeView->sortByColumn(0, Qt::AscendingOrder);
    mTreeView->header()->setSectionsClickable(true);

    auto mainLayout = new QVBoxLayout(this);
    mainLayout->setObjectName(QStringLiteral("mainLayout"));
    mainLayout->setContentsMargins({});

    mSearchLineWidget->setObjectName(QStringLiteral("mSearchLineWidget"));
    mSearchLineWidget->setPlaceholderText(i18n("Search permissions..."));
    new LineEditCatchReturnKey(mSearchLineWidget, this);
    mSearchLineWidget->setClearButtonEnabled(true);
    mainLayout->addWidget(mSearchLineWidget);
    mTreeView->setObjectName(QStringLiteral("mTreeView"));
    mainLayout->addWidget(mTreeView);
    mPermissionFilterProxyModel->setSourceModel(mAdminPermissionsModel);
    mTreeView->setModel(mPermissionFilterProxyModel);
    mTreeView->setColumnHidden(PermissionsModel::Roles, true);
    connect(mSearchLineWidget, &QLineEdit::textChanged, this, &ExplorePermissionWidget::slotFilterTextChanged);
}

ExplorePermissionWidget::~ExplorePermissionWidget() = default;

void ExplorePermissionWidget::slotFilterTextChanged(const QString &str)
{
    mPermissionFilterProxyModel->setFilterFixedString(str);
}

void ExplorePermissionWidget::setPermissions(const QVector<Permission> &permissions)
{
    Permissions perms;
    perms.setPermissions(permissions);
    mAdminPermissionsModel->setPermissions(perms);
}
