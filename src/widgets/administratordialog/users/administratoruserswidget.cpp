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
#include <KLocalizedString>
#include <QHeaderView>
#include <QLineEdit>
#include <QTableView>
#include <QVBoxLayout>

AdministratorUsersWidget::AdministratorUsersWidget(QWidget *parent)
    : QWidget(parent)
    , mSearchLineEdit(new QLineEdit(this))
    , mResultTreeWidget(new QTableView(this))
{
    auto mainLayout = new QVBoxLayout(this);
    mainLayout->setObjectName(QStringLiteral("mainLayout"));

    mSearchLineEdit->setObjectName(QStringLiteral("mSearchLineEdit"));
    mSearchLineEdit->setPlaceholderText(i18n("Search users..."));
    mSearchLineEdit->setClearButtonEnabled(true);
    new LineEditCatchReturnKey(mSearchLineEdit, this);
    connect(mSearchLineEdit, &QLineEdit::textChanged, this, &AdministratorUsersWidget::slotTextChanged);
    mainLayout->addWidget(mSearchLineEdit);

    mResultTreeWidget->setShowGrid(false);
    mResultTreeWidget->setSortingEnabled(true);
    mResultTreeWidget->setObjectName(QStringLiteral("mResultTreeWidget"));
    mResultTreeWidget->verticalHeader()->hide();
    mResultTreeWidget->setSelectionBehavior(QAbstractItemView::SelectRows);
    mResultTreeWidget->setSelectionMode(QAbstractItemView::SelectionMode::SingleSelection);
    mainLayout->addWidget(mResultTreeWidget);

    mAdminUsersModel = new AdminUsersModel(this);
    mAdminUsersModel->setObjectName(QStringLiteral("mAdminUsersModel"));

    mAdminUsersProxyModel = new AdminUsersFilterProxyModel(mAdminUsersModel, this);
    mAdminUsersProxyModel->setObjectName(QStringLiteral("mAdminUsersProxyModel"));

    mResultTreeWidget->setModel(mAdminUsersProxyModel);
}

AdministratorUsersWidget::~AdministratorUsersWidget()
{
}

void AdministratorUsersWidget::slotTextChanged(const QString &str)
{
    mAdminUsersProxyModel->setFilterString(str);
}
