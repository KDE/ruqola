/*
   SPDX-FileCopyrightText: 2023-2025 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "explorepermissionswidget.h"
using namespace Qt::Literals::StringLiterals;

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
    mPermissionFilterProxyModel->setObjectName(u"permissionFilterProxyModel"_s);
    mTreeView->setRootIsDecorated(false);
    mTreeView->setSortingEnabled(true);
    mTreeView->sortByColumn(0, Qt::AscendingOrder);
    mTreeView->header()->setSectionsClickable(true);

    auto mainLayout = new QVBoxLayout(this);
    mainLayout->setObjectName(u"mainLayout"_s);
    mainLayout->setContentsMargins({});

    mOwnRoles->setObjectName(u"mOwnRoles"_s);
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
    mOwnRoles->setText(u"<qt><b>%1</b> %2</qt>"_s.arg(i18n("Own Roles:"), roleStr.join(u',')));
}

#include "moc_explorepermissionswidget.cpp"
