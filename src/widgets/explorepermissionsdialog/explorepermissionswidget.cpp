/*
   SPDX-FileCopyrightText: 2023-2025 Laurent Montel <montel@kde.org>

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
    mSearchLineWidget->setPlaceholderText(i18nc("@info:placeholder", "Search permissions…"));
    KLineEditEventHandler::catchReturnKey(mSearchLineWidget);
    mSearchLineWidget->setClearButtonEnabled(true);
    mainLayout->addWidget(mSearchLineWidget);
    mTreeView->setObjectName(QStringLiteral("mTreeView"));
    mainLayout->addWidget(mTreeView);
    mPermissionFilterProxyModel->setSourceModel(mAdminPermissionsModel);
    mTreeView->setModel(mPermissionFilterProxyModel);
    mTreeView->setColumnHidden(PermissionsModel::Roles, true);
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
    mOwnRoles->setText(QStringLiteral("<qt><b>%1</b> %2</qt>").arg(i18n("Own Roles:"), roleStr.join(QLatin1Char(','))));
}

#include "moc_explorepermissionswidget.cpp"
