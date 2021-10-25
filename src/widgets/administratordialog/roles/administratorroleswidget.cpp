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
#include "misc/lineeditcatchreturnkey.h"
#include "model/adminrolesmodel.h"
#include "rocketchataccount.h"
#include "rolestreeview.h"
#include "ruqola.h"

#include <KLocalizedString>

#include <QLineEdit>
#include <QMenu>
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

    connect(mSearchLineWidget, &QLineEdit::textChanged, this, &AdministratorRolesWidget::slotFilterTextChanged);
    connect(mTreeView, &QTreeView::customContextMenuRequested, this, &AdministratorRolesWidget::slotCustomContextMenuRequested);
}

AdministratorRolesWidget::~AdministratorRolesWidget()
{
}

void AdministratorRolesWidget::initialize()
{
    // First load list of roles.
    auto *rcAccount = Ruqola::self()->rocketChatAccount();
    mAdminRolesModel->setRoles(rcAccount->roleInfo());
}

void AdministratorRolesWidget::slotFilterTextChanged(const QString &str)
{
    mRoleFilterProxyModel->setFilterFixedString(str);
}

void AdministratorRolesWidget::slotCustomContextMenuRequested(const QPoint &pos)
{
    const QModelIndex index = mTreeView->indexAt(pos);
#if 0
    //auto *rcAccount = Ruqola::self()->rocketChatAccount();
    //if (rcAccount->hasPermission(QStringLiteral("access-permissions"))) {
            QMenu menu(this);
            menu.addAction(QIcon::fromTheme(QStringLiteral("list-add")), i18n("Add..."), this, &AdministratorRolesWidget::addClicked);

        if (index.isValid()) {
            menu.addAction(QIcon::fromTheme(QStringLiteral("document-edit")), i18n("Modify..."), this, [this, index]() {
                modifyRoles(index);
            });
            menu.exec(mTreeView->viewport()->mapToGlobal(pos));
        }
    //}
#endif
}
