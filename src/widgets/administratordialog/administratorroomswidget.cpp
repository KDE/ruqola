/*
   Copyright (c) 2020 Laurent Montel <montel@kde.org>

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
#include "ruqola.h"
#include "rocketchataccount.h"
#include "restapirequest.h"
#include "rooms/adminroomsjob.h"
#include "ruqolawidgets_debug.h"
#include "administrator/adminrooms.h"
#include "misc/lineeditcatchreturnkey.h"
#include "model/adminroomsmodel.h"
#include "model/adminroomsfilterproxymodel.h"

#include <QVBoxLayout>
#include <KLocalizedString>
#include <QJsonObject>
#include <QLineEdit>
#include <QTableView>
#include <QHeaderView>

AdministratorRoomsWidget::AdministratorRoomsWidget(QWidget *parent)
    : QWidget(parent)
{
    QVBoxLayout *mainLayout = new QVBoxLayout(this);
    mainLayout->setObjectName(QStringLiteral("mainLayout"));
    mainLayout->setContentsMargins({});

    mSearchLineEdit = new QLineEdit(this);
    mSearchLineEdit->setObjectName(QStringLiteral("mSearchLineEdit"));
    mSearchLineEdit->setClearButtonEnabled(true);
    new LineEditCatchReturnKey(mSearchLineEdit, this);
    connect(mSearchLineEdit, &QLineEdit::textChanged, this, &AdministratorRoomsWidget::slotTextChanged);
    mainLayout->addWidget(mSearchLineEdit);

    mResultTreeWidget = new QTableView(this);
    mResultTreeWidget->setShowGrid(false);
    mResultTreeWidget->setSortingEnabled(true);
    mResultTreeWidget->setObjectName(QStringLiteral("mResultTreeWidget"));
    mResultTreeWidget->verticalHeader()->hide();
    mainLayout->addWidget(mResultTreeWidget);

    mAdminRoomsModel = new AdminRoomsModel(this);
    mAdminRoomsModel->setObjectName(QStringLiteral("mAdminRoomsModel"));

    mAdminRoomsProxyModel = new AdminRoomsFilterProxyModel(mAdminRoomsModel, this);
    mAdminRoomsProxyModel->setObjectName(QStringLiteral("mAdminRoomsProxyModel"));

    mResultTreeWidget->setModel(mAdminRoomsProxyModel);

    initialize();
}

AdministratorRoomsWidget::~AdministratorRoomsWidget()
{
}

void AdministratorRoomsWidget::slotTextChanged(const QString &text)
{
    mAdminRoomsProxyModel->setFilterString(text);
}

void AdministratorRoomsWidget::initialize()
{
    auto *rcAccount = Ruqola::self()->rocketChatAccount();
    RocketChatRestApi::AdminRoomsJob *adminRoomsJob = new RocketChatRestApi::AdminRoomsJob(this);
    rcAccount->restApi()->initializeRestApiJob(adminRoomsJob);
    connect(adminRoomsJob, &RocketChatRestApi::AdminRoomsJob::adminRoomsDone,
            this, &AdministratorRoomsWidget::slotAdminRoomDone);
    if (!adminRoomsJob->start()) {
        qCDebug(RUQOLAWIDGETS_LOG) << "Impossible to start AdminRoomsJob";
    }
}

void AdministratorRoomsWidget::slotAdminRoomDone(const QJsonObject &obj)
{
    AdminRooms rooms;
    rooms.parseAdminRooms(obj);
    mAdminRoomsModel->setAdminRooms(rooms);
    mResultTreeWidget->resizeColumnsToContents();
    //qDebug() << " rooms " << rooms;
}
