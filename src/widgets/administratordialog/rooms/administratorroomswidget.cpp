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
#include "connection.h"
#include "misc/lineeditcatchreturnkey.h"
#include "misc/searchwithdelaylineedit.h"
#include "model/adminroomsmodel.h"
#include "rocketchataccount.h"
#include "roominfo/roomsinfo.h"
#include "rooms/adminroomsjob.h"
#include "ruqola.h"
#include "ruqolawidgets_debug.h"

#include <KLocalizedString>
#include <KMessageBox>
#include <QLabel>
#include <QMenu>
#include <QTreeView>
#include <QVBoxLayout>

AdministratorRoomsWidget::AdministratorRoomsWidget(QWidget *parent)
    : SearchTreeBaseWidget(parent)
    , mSelectRoomType(new AdministratorRoomsSelectRoomTypeWidget(this))
{
    mSelectRoomType->setObjectName(QStringLiteral("mSelectRoomType"));
    connect(mSelectRoomType, &AdministratorRoomsSelectRoomTypeWidget::filterChanged, this, &AdministratorRoomsWidget::slotFilterChanged);

    mModel = new AdminRoomsModel(this);
    mModel->setObjectName(QStringLiteral("mAdminRoomsModel"));

    mProxyModelModel = new AdminRoomsFilterProxyModel(mModel, this);
    mProxyModelModel->setObjectName(QStringLiteral("mAdminUsersProxyModel"));
    mSearchLineEdit->setPlaceholderText(i18n("Search Users"));
    mTreeView->setModel(mProxyModelModel);
    mSearchLayout->addWidget(mSelectRoomType);
    hideColumns();
    connectModel();
}

AdministratorRoomsWidget::~AdministratorRoomsWidget()
{
}

void AdministratorRoomsWidget::slotCustomContextMenuRequested(const QPoint &pos)
{
    QMenu menu(this);
    const QModelIndex index = mTreeView->indexAt(pos);
    if (index.isValid()) {
        menu.addAction(QIcon::fromTheme(QStringLiteral("document-edit")), i18n("Modify..."), this, [this, index]() {
            slotModifyRoom(index);
        });
        menu.addSeparator();
        menu.addAction(QIcon::fromTheme(QStringLiteral("list-remove")), i18n("Remove"), this, [this, index]() {
            slotRemoveRoom(index);
        });
    }
    menu.exec(mTreeView->viewport()->mapToGlobal(pos));
}

void AdministratorRoomsWidget::updateLabel()
{
    mLabelResultSearch->setText(mModel->total() == 0 ? i18n("No room found") : displayShowMessageInRoom());
}

QString AdministratorRoomsWidget::displayShowMessageInRoom() const
{
    QString displayMessageStr = i18np("%1 room (Total: %2)", "%1 rooms (Total: %2)", mModel->rowCount(), mModel->total());
    if (!mModel->hasFullList()) {
        displayMessageStr += clickableStr();
    }
    return displayMessageStr;
}

void AdministratorRoomsWidget::slotModifyRoom(const QModelIndex &index)
{
    // TODO
}

void AdministratorRoomsWidget::slotRemoveRoom(const QModelIndex &index)
{
    const QString roomName = index.data(AdminRoomsModel::Name).toString();
    if (KMessageBox::Yes == KMessageBox::questionYesNo(this, i18n("Do you want to remove \"%1\"?", roomName), i18nc("@title", "Remove Room"))) {
        qWarning() << " NOT IMPLEMENTED";
    }
}

void AdministratorRoomsWidget::slotFilterChanged(AdminRoomsFilterProxyModel::FilterRooms filters)
{
    static_cast<AdminRoomsFilterProxyModel *>(mProxyModelModel)->setFilterRooms(filters);
}

void AdministratorRoomsWidget::slotLoadElements(int offset, int count, const QString &searchName)
{
    auto *rcAccount = Ruqola::self()->rocketChatAccount();
    auto job = new RocketChatRestApi::AdminRoomsJob(this);
    RocketChatRestApi::AdminRoomsJob::AdminRoomsJobInfo info;
    info.filter = searchName;
    info.searchType = RocketChatRestApi::AdminRoomsJob::AdminRoomSearchType::All;
    job->setRoomsAdminInfo(info);

    RocketChatRestApi::QueryParameters parameters;
    QMap<QString, RocketChatRestApi::QueryParameters::SortOrder> map;
    map.insert(QStringLiteral("name"), RocketChatRestApi::QueryParameters::SortOrder::Ascendant);
    parameters.setSorting(map);
    if (offset != -1) {
        parameters.setOffset(offset);
    }
    if (count != -1) {
        parameters.setCount(count);
    }
    job->setQueryParameters(parameters);

    rcAccount->restApi()->initializeRestApiJob(job);
    if (offset != -1) {
        connect(job, &RocketChatRestApi::AdminRoomsJob::adminRoomsDone, this, &AdministratorRoomsWidget::slotLoadMoreElementDone);
    } else {
        connect(job, &RocketChatRestApi::AdminRoomsJob::adminRoomsDone, this, &AdministratorRoomsWidget::slotSearchDone);
    }
    if (!job->start()) {
        qCWarning(RUQOLAWIDGETS_LOG) << "Impossible to start AdminRoomsJob job";
    }
}
