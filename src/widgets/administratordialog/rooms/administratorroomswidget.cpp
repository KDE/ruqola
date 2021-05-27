/*
   Copyright (c) 2020-2021 Laurent Montel <montel@kde.org>

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

#include "administratorroomswidget.h"
#include "administrator/adminrooms.h"
#include "misc/lineeditcatchreturnkey.h"
#include "model/adminroomsmodel.h"
#include "restapirequest.h"
#include "rocketchataccount.h"
#include "rooms/adminroomsjob.h"
#include "ruqola.h"
#include "ruqolawidgets_debug.h"

#include <KLocalizedString>
#include <KMessageBox>
#include <QHeaderView>
#include <QJsonObject>
#include <QLineEdit>
#include <QMenu>
#include <QTableView>
#include <QVBoxLayout>

AdministratorRoomsWidget::AdministratorRoomsWidget(QWidget *parent)
    : QWidget(parent)
    , mSearchLineEdit(new QLineEdit(this))
    , mSelectRoomType(new AdministratorRoomsSelectRoomTypeWidget(this))
    , mResultTreeWidget(new QTableView(this))
    , mAdminRoomsModel(new AdminRoomsModel(this))
{
    auto mainLayout = new QVBoxLayout(this);
    mainLayout->setObjectName(QStringLiteral("mainLayout"));

    mSearchLineEdit->setObjectName(QStringLiteral("mSearchLineEdit"));
    mSearchLineEdit->setPlaceholderText(i18n("Search channel..."));
    mSearchLineEdit->setClearButtonEnabled(true);
    new LineEditCatchReturnKey(mSearchLineEdit, this);
    connect(mSearchLineEdit, &QLineEdit::textChanged, this, &AdministratorRoomsWidget::slotTextChanged);
    mainLayout->addWidget(mSearchLineEdit);

    mSelectRoomType->setObjectName(QStringLiteral("mSelectRoomType"));
    mainLayout->addWidget(mSelectRoomType);
    connect(mSelectRoomType, &AdministratorRoomsSelectRoomTypeWidget::filterChanged, this, &AdministratorRoomsWidget::slotFilterChanged);

    mResultTreeWidget->setShowGrid(false);
    mResultTreeWidget->setSortingEnabled(true);
    mResultTreeWidget->setObjectName(QStringLiteral("mResultTreeWidget"));
    mResultTreeWidget->verticalHeader()->hide();
    mResultTreeWidget->setSelectionBehavior(QAbstractItemView::SelectRows);
    mResultTreeWidget->setSelectionMode(QAbstractItemView::SelectionMode::SingleSelection);
    mResultTreeWidget->setContextMenuPolicy(Qt::CustomContextMenu);
    connect(mResultTreeWidget, &QTableView::customContextMenuRequested, this, &AdministratorRoomsWidget::slotCustomContextMenuRequested);
    mainLayout->addWidget(mResultTreeWidget);

    mAdminRoomsModel->setObjectName(QStringLiteral("mAdminRoomsModel"));

    mAdminRoomsProxyModel = new AdminRoomsFilterProxyModel(mAdminRoomsModel, this);
    mAdminRoomsProxyModel->setObjectName(QStringLiteral("mAdminRoomsProxyModel"));

    mResultTreeWidget->setModel(mAdminRoomsProxyModel);
    // Hide not useful columns
    mResultTreeWidget->setColumnHidden(AdminRoomsModel::AdminRoomsRoles::ChannelType, true);
    mResultTreeWidget->setColumnHidden(AdminRoomsModel::AdminRoomsRoles::Identifier, true);

    initialize();
}

AdministratorRoomsWidget::~AdministratorRoomsWidget()
{
}

void AdministratorRoomsWidget::slotCustomContextMenuRequested(const QPoint &pos)
{
    QMenu menu(this);
    menu.addAction(QIcon::fromTheme(QStringLiteral("list-add")), i18n("Add..."), this, &AdministratorRoomsWidget::slotAddRoom);
    const QModelIndex index = mResultTreeWidget->indexAt(pos);
    if (index.isValid()) {
        menu.addAction(QIcon::fromTheme(QStringLiteral("document-edit")), i18n("Modify..."), this, [this, index]() {
            slotModifyRoom(index);
        });
        menu.addSeparator();
        menu.addAction(QIcon::fromTheme(QStringLiteral("list-remove")), i18n("Remove"), this, [this, index]() {
            slotRemoveRoom(index);
        });
    }
    menu.exec(mResultTreeWidget->viewport()->mapToGlobal(pos));
}

void AdministratorRoomsWidget::slotAddRoom()
{
    // TODO
}

void AdministratorRoomsWidget::slotModifyRoom(const QModelIndex &index)
{
    // TODO
}

void AdministratorRoomsWidget::slotRemoveRoom(const QModelIndex &index)
{
    const QString roomName = index.data(AdminRoomsModel::Name).toString();
    if (KMessageBox::Yes == KMessageBox::questionYesNo(this, i18n("Do you want to remove \"%1\"?", roomName), i18n("Remove Room"))) {
        qWarning() << " NOT IMPLEMENTED";
    }
}

void AdministratorRoomsWidget::slotFilterChanged(AdminRoomsFilterProxyModel::FilterRooms filters)
{
    mAdminRoomsProxyModel->setFilterRooms(filters);
}

void AdministratorRoomsWidget::slotTextChanged(const QString &text)
{
    mAdminRoomsProxyModel->setFilterString(text);
}

void AdministratorRoomsWidget::initialize()
{
    auto *rcAccount = Ruqola::self()->rocketChatAccount();
    auto adminRoomsJob = new RocketChatRestApi::AdminRoomsJob(this);
    rcAccount->restApi()->initializeRestApiJob(adminRoomsJob);
    connect(adminRoomsJob, &RocketChatRestApi::AdminRoomsJob::adminRoomsDone, this, &AdministratorRoomsWidget::slotAdminRoomDone);
    if (!adminRoomsJob->start()) {
        qCDebug(RUQOLAWIDGETS_LOG) << "Impossible to start AdminRoomsJob";
    }
}

void AdministratorRoomsWidget::slotAdminRoomDone(const QJsonObject &obj)
{
    AdminRooms rooms;
    rooms.parseRooms(obj);
    mAdminRoomsModel->setAdminRooms(rooms);
    mResultTreeWidget->resizeColumnsToContents();
    // qDebug() << " rooms " << rooms;
}
