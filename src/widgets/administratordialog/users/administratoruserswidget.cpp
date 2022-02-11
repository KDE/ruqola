/*
   SPDX-FileCopyrightText: 2021 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "administratoruserswidget.h"
#include "administratoradduserdialog.h"
#include "connection.h"
#include "misc/searchwithdelaylineedit.h"
#include "model/adminusersmodel.h"
#include "model/directorybasefilterproxymodel.h"
#include "rocketchataccount.h"
#include "ruqola.h"
#include "ruqolawidgets_debug.h"
#include "users/deleteuserjob.h"
#include "users/setuseractivestatusjob.h"
#include "users/userinfojob.h"
#include "users/userscreatejob.h"
#include "users/userslistjob.h"
#include "users/usersupdatejob.h"
#include <KLocalizedString>
#include <KMessageBox>
#include <QLabel>
#include <QMenu>
#include <QPointer>
#include <QTreeView>

AdministratorUsersWidget::AdministratorUsersWidget(RocketChatAccount *account, QWidget *parent)
    : SearchTreeBaseWidget(account, parent)
{
    mModel = new AdminUsersModel(this);
    mModel->setObjectName(QStringLiteral("mAdminUsersModel"));

    mProxyModelModel = new DirectoryBaseFilterProxyModel(mModel, this);
    mProxyModelModel->setObjectName(QStringLiteral("mAdminUsersProxyModel"));
    mSearchLineEdit->setPlaceholderText(i18n("Search Users"));
    mTreeView->setModel(mProxyModelModel);
    connect(mTreeView, &QTreeView::doubleClicked, this, &AdministratorUsersWidget::slotModifyDoubleClickUser);
    hideColumns();
    connectModel();
}

AdministratorUsersWidget::~AdministratorUsersWidget() = default;

void AdministratorUsersWidget::slotTextChanged(const QString &str)
{
    mProxyModelModel->setFilterString(str);
}

void AdministratorUsersWidget::slotAddUser()
{
    QPointer<AdministratorAddUserDialog> dlg = new AdministratorAddUserDialog(this);
    dlg->setRoleInfo(mRocketChatAccount->roleInfo());
    if (dlg->exec()) {
        const RocketChatRestApi::CreateUpdateUserInfo info = dlg->createInfo();
        auto job = new RocketChatRestApi::UsersCreateJob(this);
        job->setCreateInfo(info);
        mRocketChatAccount->restApi()->initializeRestApiJob(job);
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
    // TODO use obj ? Add it in list
}

void AdministratorUsersWidget::slotModifyDoubleClickUser(const QModelIndex &index)
{
    if (index.isValid()) {
        const QModelIndex newModelIndex = mProxyModelModel->mapToSource(index);
        const QModelIndex modelIndex = mModel->index(newModelIndex.row(), AdminUsersModel::UserId);
        slotModifyUser(modelIndex);
    }
}

void AdministratorUsersWidget::slotModifyUser(const QModelIndex &index)
{
    auto userJob = new RocketChatRestApi::UserInfoJob(this);
    mRocketChatAccount->restApi()->initializeRestApiJob(userJob);
    RocketChatRestApi::UserBaseJob::UserInfo info;
    info.userInfoType = RocketChatRestApi::UserBaseJob::UserInfoType::UserId;
    const QString userId = index.data().toString();
    info.userIdentifier = userId;
    userJob->setUserInfo(info);
    connect(userJob, &RocketChatRestApi::UserInfoJob::userInfoDone, this, &AdministratorUsersWidget::slotUserInfoDone);
    if (!userJob->start()) {
        qCDebug(RUQOLAWIDGETS_LOG) << "Impossible to start UserInfoJob";
    }
}

void AdministratorUsersWidget::slotUserInfoDone(const QJsonObject &obj)
{
    QPointer<AdministratorAddUserDialog> dlg = new AdministratorAddUserDialog(this);
    dlg->setRoleInfo(mRocketChatAccount->roleInfo());
    User user;
    user.parseUserRestApi(obj[QLatin1String("user")].toObject());
    dlg->setUser(user);
    if (dlg->exec()) {
        RocketChatRestApi::UpdateUserInfo info = dlg->updateInfo();
        info.mUserId = user.userId();
        auto job = new RocketChatRestApi::UsersUpdateJob(this);
        job->setUpdateInfo(info);
        mRocketChatAccount->restApi()->initializeRestApiJob(job);
        connect(job, &RocketChatRestApi::UsersUpdateJob::usersUpdateDone, this, &AdministratorUsersWidget::slotUserUpdateDone);
        if (!job->start()) {
            qCWarning(RUQOLAWIDGETS_LOG) << "Impossible to start UsersUpdateJob job";
        }
    }
    delete dlg;
}

void AdministratorUsersWidget::slotUserUpdateDone(const QJsonObject &obj)
{
    User newUser;
    newUser.parseUserRestApi(obj[QLatin1String("user")].toObject());
    // TODO update user;
    qDebug() << " obj " << obj;
}

void AdministratorUsersWidget::slotRemoveUser(const QModelIndex &index)
{
    if (KMessageBox::questionYesNo(this, i18n("Do you want to remove this user?"), i18n("Remove User"), KStandardGuiItem::remove(), KStandardGuiItem::cancel())
        == KMessageBox::Yes) {
        auto job = new RocketChatRestApi::DeleteUserJob(this);
        RocketChatRestApi::UserBaseJob::UserInfo info;
        info.userInfoType = RocketChatRestApi::UserBaseJob::UserInfoType::UserId;
        const QString userId = index.data().toString();
        info.userIdentifier = userId;
        job->setUserInfo(info);
        mRocketChatAccount->restApi()->initializeRestApiJob(job);
        connect(job, &RocketChatRestApi::DeleteUserJob::deleteUserDone, this, [this, userId]() {
            slotDeleteUserDone(userId);
        });
        if (!job->start()) {
            qCWarning(RUQOLAWIDGETS_LOG) << "Impossible to start DeleteUserJob job";
        }
    }
}

void AdministratorUsersWidget::slotDeleteUserDone(const QString &userId)
{
    mModel->removeElement(userId);
}

void AdministratorUsersWidget::slotActivateUser(const QModelIndex &index, bool activateUser)
{
    auto job = new RocketChatRestApi::SetUserActiveStatusJob(this);
    const QModelIndex modelIndex = mModel->index(index.row(), AdminUsersModel::UserId);
    const QString userId = modelIndex.data().toString();
    job->setActivate(!activateUser);
    job->setActivateUserId(userId);
    mRocketChatAccount->restApi()->initializeRestApiJob(job);
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
    // const QString userId = userObj[QLatin1String("_id")].toString();
    mModel->setData(modelIndex, active, AdminUsersModel::ActiveUser);
}

void AdministratorUsersWidget::slotCustomContextMenuRequested(const QPoint &pos)
{
    QMenu menu(this);
    menu.addAction(QIcon::fromTheme(QStringLiteral("list-add")), i18n("Add..."), this, &AdministratorUsersWidget::slotAddUser);
    const QModelIndex index = mTreeView->indexAt(pos);
    if (index.isValid()) {
        const QModelIndex newModelIndex = mProxyModelModel->mapToSource(index);

        const QModelIndex modelIndex = mModel->index(newModelIndex.row(), AdminUsersModel::ActiveUser);
        menu.addAction(QIcon::fromTheme(QStringLiteral("document-edit")), i18n("Modify..."), this, [this, newModelIndex]() {
            const QModelIndex modelIndex = mModel->index(newModelIndex.row(), AdminUsersModel::UserId);
            slotModifyUser(modelIndex);
        });
        menu.addSeparator();
        const bool activateUser = modelIndex.data().toBool();
        menu.addAction(activateUser ? i18n("Disable") : i18n("Active"), this, [this, newModelIndex, activateUser]() {
            slotActivateUser(newModelIndex, activateUser);
        });
        menu.addSeparator();
        // Add support for reset E2E/TOTP etc.

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
        displayMessageStr += clickableStr();
    }
    return displayMessageStr;
}

void AdministratorUsersWidget::slotLoadElements(int offset, int count, const QString &searchName)
{
    auto job = new RocketChatRestApi::UsersListJob(this);
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
    parameters.setSearchString(searchName);
    job->setQueryParameters(parameters);

    mRocketChatAccount->restApi()->initializeRestApiJob(job);
    if (offset != -1) {
        connect(job, &RocketChatRestApi::UsersListJob::userListDone, this, &AdministratorUsersWidget::slotLoadMoreElementDone);
    } else {
        connect(job, &RocketChatRestApi::UsersListJob::userListDone, this, &AdministratorUsersWidget::slotSearchDone);
    }
    if (!job->start()) {
        qCWarning(RUQOLAWIDGETS_LOG) << "Impossible to start searchRoomUser job";
    }
}
