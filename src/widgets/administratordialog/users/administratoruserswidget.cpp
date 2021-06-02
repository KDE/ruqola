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
#include "misc/searchwithdelaylineedit.h"
#include "model/adminusersfilterproxymodel.h"
#include "model/adminusersmodel.h"
#include "restapirequest.h"
#include "rocketchataccount.h"
#include "ruqola.h"
#include "ruqolawidgets_debug.h"
#include "users/userslistjob.h"
#include <KLocalizedString>
#include <QHeaderView>
#include <QLabel>
#include <QLineEdit>
#include <QMenu>
#include <QTreeView>
#include <QVBoxLayout>

AdministratorUsersWidget::AdministratorUsersWidget(QWidget *parent)
    : SearchTreeBaseWidget(parent)
    , mAdminUsersModel(new AdminUsersModel(this))
{
    // connect(mResultTreeView, &QTreeView::customContextMenuRequested, this, &AdministratorUsersWidget::slotCustomContextMenuRequested);

    mAdminUsersModel->setObjectName(QStringLiteral("mAdminUsersModel"));

    mAdminUsersProxyModel = new AdminUsersFilterProxyModel(mAdminUsersModel, this);
    mAdminUsersProxyModel->setObjectName(QStringLiteral("mAdminUsersProxyModel"));

    mTreeView->setModel(mAdminUsersProxyModel);
    connect(mTreeView, &QTreeView::customContextMenuRequested, this, &AdministratorUsersWidget::slotCustomContextMenuRequested);
    connect(mAdminUsersModel, &DirectoryBaseModel::hasFullListChanged, this, &AdministratorUsersWidget::updateLabel);
    connect(mAdminUsersModel, &DirectoryBaseModel::totalChanged, this, &AdministratorUsersWidget::updateLabel);
    connect(mAdminUsersModel, &DirectoryBaseModel::loadingInProgressChanged, this, &AdministratorUsersWidget::updateLabel);
    connect(mSearchLineEdit, &SearchWithDelayLineEdit::searchCleared, this, &AdministratorUsersWidget::slotSearchCleared);
    connect(mSearchLineEdit, &SearchWithDelayLineEdit::searchRequested, this, &AdministratorUsersWidget::slotSearchRequested);
}

AdministratorUsersWidget::~AdministratorUsersWidget()
{
}

void AdministratorUsersWidget::slotTextChanged(const QString &str)
{
    mAdminUsersProxyModel->setFilterString(str);
}

void AdministratorUsersWidget::initialize()
{
    slotLoadElements();
}

void AdministratorUsersWidget::slotLoadMoreElements()
{
    if (!mAdminUsersModel->loadMoreInProgress()) {
        const int offset = mAdminUsersModel->rowCount();
        if (offset < mAdminUsersModel->total()) {
            mAdminUsersModel->setLoadMoreInProgress(true);
            // slotLoadElements(offset, qMin(50, mAdminUsersModel->total() - offset), mSearchLineEdit->text().trimmed());
        }
    }
}

void AdministratorUsersWidget::finishSearching()
{
    mAdminUsersModel->setLoadMoreInProgress(false);
    mTreeView->header()->resizeSections(QHeaderView::ResizeToContents);
}

void AdministratorUsersWidget::slotAddUser()
{
}

void AdministratorUsersWidget::slotModifyUser(const QModelIndex &index)
{
}

void AdministratorUsersWidget::slotRemoveUser(const QModelIndex &index)
{
}

void AdministratorUsersWidget::slotCustomContextMenuRequested(const QPoint &pos)
{
    QMenu menu(this);
    menu.addAction(QIcon::fromTheme(QStringLiteral("list-add")), i18n("Add..."), this, &AdministratorUsersWidget::slotAddUser);
    const QModelIndex index = mTreeView->indexAt(pos);
    if (index.isValid()) {
        menu.addAction(QIcon::fromTheme(QStringLiteral("document-edit")), i18n("Modify..."), this, [this, index]() {
            slotModifyUser(index);
        });
        menu.addSeparator();
        menu.addAction(QIcon::fromTheme(QStringLiteral("list-remove")), i18n("Remove"), this, [this, index]() {
            slotRemoveUser(index);
        });
    }
    menu.exec(mTreeView->viewport()->mapToGlobal(pos));
}

void AdministratorUsersWidget::updateLabel()
{
    mLabelResultSearch->setText(mAdminUsersModel->total() == 0 ? i18n("No user found") : displayShowMessageInRoom());
}

QString AdministratorUsersWidget::displayShowMessageInRoom() const
{
    QString displayMessageStr = i18np("%1 user (Total: %2)", "%1 users (Total: %2)", mAdminUsersModel->rowCount(), mAdminUsersModel->total());
    if (!mAdminUsersModel->hasFullList()) {
        displayMessageStr += QStringLiteral(" <a href=\"loadmoreelement\">%1</a>").arg(i18n("(Click here for Loading more...)"));
    }
    return displayMessageStr;
}

void AdministratorUsersWidget::slotSearchCleared()
{
    slotLoadElements();
}

void AdministratorUsersWidget::slotSearchRequested(const QString &str)
{
    slotLoadElements(-1, -1, str);
}

void AdministratorUsersWidget::slotLoadElements(int offset, int count, const QString &searchName)
{
    auto *rcAccount = Ruqola::self()->rocketChatAccount();
    auto job = new RocketChatRestApi::UsersListJob(this);
    //    if (!searchName.isEmpty()) {
    //        info.pattern = searchName;
    //    }
    // job->setDirectoryInfo(info);
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
        connect(job, &RocketChatRestApi::UsersListJob::userListDone, this, &AdministratorUsersWidget::slotLoadMoreElementDone);
    } else {
        connect(job, &RocketChatRestApi::UsersListJob::userListDone, this, &AdministratorUsersWidget::slotSearchDone);
    }
    if (!job->start()) {
        qCWarning(RUQOLAWIDGETS_LOG) << "Impossible to start searchRoomUser job";
    }
}

void AdministratorUsersWidget::slotLoadMoreElementDone(const QJsonObject &obj)
{
    mAdminUsersModel->addMoreElements(obj);
    finishSearching();
}

void AdministratorUsersWidget::slotSearchDone(const QJsonObject &obj)
{
    mAdminUsersModel->parseElements(obj);
    finishSearching();
}
