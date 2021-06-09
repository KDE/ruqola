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
#include "administratoradduserdialog.h"
#include "misc/lineeditcatchreturnkey.h"
#include "misc/searchwithdelaylineedit.h"
#include "model/adminusersfilterproxymodel.h"
#include "model/adminusersmodel.h"
#include "restapirequest.h"
#include "rocketchataccount.h"
#include "ruqola.h"
#include "ruqolawidgets_debug.h"
#include "users/deleteuserjob.h"
#include "users/setuseractivestatusjob.h"
#include "users/userscreatejob.h"
#include "users/userslistjob.h"
#include <KLocalizedString>
#include <QHeaderView>
#include <QLabel>
#include <QLineEdit>
#include <QMenu>
#include <QMessageBox>
#include <QPointer>
#include <QTreeView>
#include <QVBoxLayout>

AdministratorUsersWidget::AdministratorUsersWidget(QWidget *parent)
    : SearchTreeBaseWidget(parent)
{
    mModel = new AdminUsersModel(this);
    mModel->setObjectName(QStringLiteral("mAdminUsersModel"));

    mAdminUsersProxyModel = new AdminUsersFilterProxyModel(mModel, this);
    mAdminUsersProxyModel->setObjectName(QStringLiteral("mAdminUsersProxyModel"));
    mSearchLineEdit->setPlaceholderText(i18n("Search Users"));
    mTreeView->setModel(mAdminUsersProxyModel);
    hideColumns();
    connectModel();
}

AdministratorUsersWidget::~AdministratorUsersWidget()
{
}

void AdministratorUsersWidget::slotTextChanged(const QString &str)
{
    mAdminUsersProxyModel->setFilterString(str);
}

void AdministratorUsersWidget::slotAddUser()
{
    QPointer<AdministratorAddUserDialog> dlg = new AdministratorAddUserDialog(this);
    if (dlg->exec()) {
        const RocketChatRestApi::CreateUpdateUserInfo info = dlg->createInfo();
        auto *rcAccount = Ruqola::self()->rocketChatAccount();
        auto job = new RocketChatRestApi::UsersCreateJob(this);
        job->setCreateInfo(info);
        rcAccount->restApi()->initializeRestApiJob(job);
        connect(job, &RocketChatRestApi::UsersCreateJob::usersCreateDone, this, &AdministratorUsersWidget::slotUserCreateDone);
        if (!job->start()) {
            qCWarning(RUQOLAWIDGETS_LOG) << "Impossible to start UsersCreateJob job";
        }
    }
    delete dlg;
}

void AdministratorUsersWidget::slotUserCreateDone(const QJsonObject &obj)
{
    qDebug() << "obj" << obj;
    // TODO use obj ?
}

void AdministratorUsersWidget::slotModifyUser(const QModelIndex &index)
{
    // TODO
    QPointer<AdministratorAddUserDialog> dlg = new AdministratorAddUserDialog(this);
    if (dlg->exec()) {
        //        const RocketChatRestApi::UsersCreateJob::CreateInfo info = dlg->createInfo();
        //        auto *rcAccount = Ruqola::self()->rocketChatAccount();
        //        auto job = new RocketChatRestApi::UsersCreateJob(this);
        //        job->setCreateInfo(info);
        //        rcAccount->restApi()->initializeRestApiJob(job);
        //        connect(job, &RocketChatRestApi::UsersCreateJob::usersCreateDone, this, &AdministratorUsersWidget::slotUserCreateDone);
        //        if (!job->start()) {
        //            qCWarning(RUQOLAWIDGETS_LOG) << "Impossible to start UsersCreateJob job";
        //        }
    }
    delete dlg;
}

void AdministratorUsersWidget::slotRemoveUser(const QModelIndex &index)
{
    if (QMessageBox::question(this, i18n("Remove User"), i18n("Do you want to remove this user?")) == QMessageBox::Yes) {
        auto *rcAccount = Ruqola::self()->rocketChatAccount();
        auto job = new RocketChatRestApi::DeleteUserJob(this);
        RocketChatRestApi::UserBaseJob::UserInfo info;
        info.userInfoType = RocketChatRestApi::UserBaseJob::UserInfoType::UserId;
        const QString userId = index.data().toString();
        info.userIdentifier = userId;
        job->setUserInfo(info);
        rcAccount->restApi()->initializeRestApiJob(job);
        connect(job, &RocketChatRestApi::DeleteUserJob::deleteUserDone, this, [this, userId]() {
            slotDeleteUserDone(userId);
        });
        if (!job->start()) {
            qCWarning(RUQOLAWIDGETS_LOG) << "Impossible to start UsersCreateJob job";
        }
    }
}

void AdministratorUsersWidget::slotDeleteUserDone(const QString &userId)
{
    mModel->removeElement(userId);
}

void AdministratorUsersWidget::slotActivateUser(const QModelIndex &index, bool activateUser)
{
    auto *rcAccount = Ruqola::self()->rocketChatAccount();
    auto job = new RocketChatRestApi::SetUserActiveStatusJob(this);
    const QModelIndex modelIndex = mModel->index(index.row(), AdminUsersModel::UserId);
    const QString userId = modelIndex.data().toString();
    job->setActivate(!activateUser);
    job->setActivateUserId(userId);
    rcAccount->restApi()->initializeRestApiJob(job);
    connect(job, &RocketChatRestApi::SetUserActiveStatusJob::setUserActiveStatusDone, this, [this, modelIndex](const QJsonObject &replyObject) {
        slotSetUserActiveStatus(replyObject, modelIndex);
    });
    if (!job->start()) {
        qCWarning(RUQOLAWIDGETS_LOG) << "Impossible to start SetUserActiveStatusJob job";
    }
}

void AdministratorUsersWidget::slotSetUserActiveStatus(const QJsonObject &replyObject, const QModelIndex &modelIndex)
{
    const QJsonObject userObj = replyObject[QLatin1String("user")].toObject();
    const bool active = userObj[QLatin1String("active")].toBool();
    const QString userId = userObj[QLatin1String("_id")].toString();
    mModel->setData(modelIndex, active, AdminUsersModel::ActiveUser);
}

void AdministratorUsersWidget::slotCustomContextMenuRequested(const QPoint &pos)
{
    QMenu menu(this);
    menu.addAction(QIcon::fromTheme(QStringLiteral("list-add")), i18n("Add..."), this, &AdministratorUsersWidget::slotAddUser);
    const QModelIndex index = mTreeView->indexAt(pos);
    if (index.isValid()) {
        const QModelIndex newModelIndex = mAdminUsersProxyModel->mapToSource(index);

        const QModelIndex modelIndex = mModel->index(newModelIndex.row(), AdminUsersModel::ActiveUser);
        const bool activateUser = modelIndex.data().toBool();
        menu.addAction(activateUser ? i18n("Disable") : i18n("Active"), this, [this, newModelIndex, activateUser]() {
            slotActivateUser(newModelIndex, activateUser);
        });
        menu.addSeparator();
        menu.addAction(QIcon::fromTheme(QStringLiteral("document-edit")), i18n("Modify..."), this, [this, index]() {
            slotModifyUser(index);
        });
        menu.addSeparator();
        menu.addAction(QIcon::fromTheme(QStringLiteral("list-remove")), i18n("Remove"), this, [this, newModelIndex]() {
            const QModelIndex i = mModel->index(newModelIndex.row(), AdminUsersModel::UserId);
            slotRemoveUser(i);
        });
    }
    menu.exec(mTreeView->viewport()->mapToGlobal(pos));
}

void AdministratorUsersWidget::updateLabel()
{
    mLabelResultSearch->setText(mModel->total() == 0 ? i18n("No user found") : displayShowMessageInRoom());
}

QString AdministratorUsersWidget::displayShowMessageInRoom() const
{
    QString displayMessageStr = i18np("%1 user (Total: %2)", "%1 users (Total: %2)", mModel->rowCount(), mModel->total());
    if (!mModel->hasFullList()) {
        displayMessageStr += QStringLiteral(" <a href=\"loadmoreelement\">%1</a>").arg(i18n("(Click here for Loading more...)"));
    }
    return displayMessageStr;
}

void AdministratorUsersWidget::slotLoadElements(int offset, int count, const QString &searchName)
{
    auto *rcAccount = Ruqola::self()->rocketChatAccount();
    auto job = new RocketChatRestApi::UsersListJob(this);
    if (!searchName.isEmpty()) {
        job->setPattern(searchName);
    }
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
