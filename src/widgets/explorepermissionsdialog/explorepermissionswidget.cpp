/*
   SPDX-FileCopyrightText: 2023-2024 Laurent Montel <montel.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "explorepermissionswidget.h"
#include "explorepermissionswidgetfilterproxymodel.h"
#include "model/permissionsmodel.h"
#include <KLineEditEventHandler>
#include <KLocalizedString>
#include <QHeaderView>
#include <QLabel>
#include <QLineEdit>
#include <QSortFilterProxyModel>
#include <QTreeView>
#include <QVBoxLayout>

ExplorePermissionsWidget::ExplorePermissionsWidget(QWidget *parent)
    : QWidget{parent}
    , mTreeView(new QTreeView(this))
    , mSearchLineWidget(new QLineEdit(this))
    , mAdminPermissionsModel(new PermissionsModel(this))
    , mPermissionFilterProxyModel(new ExplorePermissionsWidgetFilterProxyModel(this))
    , mOwnRoles(new QLabel(this))
{
    mPermissionFilterProxyModel->setObjectName(QStringLiteral("permissionFilterProxyModel"));
    mTreeView->setRootIsDecorated(false);
    mTreeView->setSortingEnabled(true);
    mTreeView->sortByColumn(0, Qt::AscendingOrder);
    mTreeView->header()->setSectionsClickable(true);

    auto mainLayout = new QVBoxLayout(this);
    mainLayout->setObjectName(QStringLiteral("mainLayout"));
    mainLayout->setContentsMargins({});

    mOwnRoles->setObjectName(QStringLiteral("mOwnRoles"));
    mSearchLineWidget->setObjectName(QStringLiteral("mSearchLineWidget"));
    mSearchLineWidget->setPlaceholderText(i18n("Search permissions..."));
    KLineEditEventHandler::catchReturnKey(mSearchLineWidget);
    mSearchLineWidget->setClearButtonEnabled(true);
    mainLayout->addWidget(mSearchLineWidget);
    mTreeView->setObjectName(QStringLiteral("mTreeView"));
    mainLayout->addWidget(mTreeView);
    mPermissionFilterProxyModel->setSourceModel(mAdminPermissionsModel);
    mTreeView->setModel(mPermissionFilterProxyModel);
    mTreeView->setColumnHidden(PermissionsModel::RolesRole, true);
    connect(mSearchLineWidget, &QLineEdit::textChanged, this, &ExplorePermissionsWidget::slotFilterTextChanged);

    mOwnRoles->setTextInteractionFlags(Qt::TextBrowserInteraction);
    mainLayout->addWidget(mOwnRoles);
}

ExplorePermissionsWidget::~ExplorePermissionsWidget() = default;

void ExplorePermissionsWidget::slotFilterTextChanged(const QString &str)
{
    mPermissionFilterProxyModel->setFilterString(str);
}

void ExplorePermissionsWidget::setPermissions(const QList<Permission> &permissions)
{
    Permissions perms;
    perms.setPermissions(permissions);
    mAdminPermissionsModel->setPermissions(perms);
    mTreeView->header()->resizeSections(QHeaderView::ResizeToContents);
}

void ExplorePermissionsWidget::setOWnRoles(const QStringList &roleStr)
{
    mOwnRoles->setText(QStringLiteral("<qt><b>Own Roles:</b> %1</qt>").arg(roleStr.join(QLatin1Char(','))));
}

#include "moc_explorepermissionswidget.cpp"
