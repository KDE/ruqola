/*
   SPDX-FileCopyrightText: 2021-2022 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "administratoruserswidget.h"
#include "administratoradduserdialog.h"
#include "connection.h"
#include "dialogs/confirmpassworddialog.h"
#include "misc/searchwithdelaylineedit.h"
#include "model/adminusersmodel.h"
#include "model/searchtreebasefilterproxymodel.h"
#include "rocketchataccount.h"
#include "ruqola.h"
#include "ruqolawidgets_debug.h"
#include "users/deleteuserjob.h"
#include "users/resete2ekeyjob.h"
#include "users/resettotpjob.h"
#include "users/setuseractivestatusjob.h"
#include "users/userinfojob.h"
#include "users/userscreatejob.h"
#include "users/userslistjob.h"
#include "users/usersupdatejob.h"
#include "utils.h"
#include <KLocalizedString>
#include <KMessageBox>
#include <QLabel>
#include <QMenu>
#include <QPointer>
#include <QTreeView>
#include <kwidgetsaddons_version.h>

AdministratorUsersWidget::AdministratorUsersWidget(RocketChatAccount *account, QWidget *parent)
    : SearchTreeBaseWidget(account, parent)
{
    auto adminUsersModel = new AdminUsersModel(this);
    adminUsersModel->setObjectName(QStringLiteral("mAdminUsersModel"));
    if (account) {
        adminUsersModel->setRoles(account->roleInfo());
    }
    mModel = adminUsersModel;

    mProxyModelModel = new SearchTreeBaseFilterProxyModel(mModel, this);
    mProxyModelModel->setObjectName(QStringLiteral("mAdminUsersProxyModel"));
    mSearchLineEdit->setPlaceholderText(i18n("Search Users..."));
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
    mModel->insertElement(obj);
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
    user.parseUserRestApi(obj[QLatin1String("user")].toObject(), mRocketChatAccount->roleInfo());
    dlg->setUser(user);
    if (dlg->exec()) {
        QString password;
        const bool twoFactorAuthenticationEnforcePasswordFallback = mRocketChatAccount->twoFactorAuthenticationEnforcePasswordFallback();
        if (twoFactorAuthenticationEnforcePasswordFallback) {
            QPointer<ConfirmPasswordDialog> dialog(new ConfirmPasswordDialog(this));
            if (dialog->exec()) {
                password = dialog->password();
            }
            delete dialog;
        }
        RocketChatRestApi::UpdateUserInfo info = dlg->updateInfo();
        info.mUserId = user.userId();
        auto job = new RocketChatRestApi::UsersUpdateJob(this);
        job->setUpdateInfo(info);
        if (twoFactorAuthenticationEnforcePasswordFallback) {
            job->setAuthMethod(QStringLiteral("password"));
            job->setAuthCode(QString::fromLatin1(Utils::convertSha256Password(password)));
        }
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
    mModel->updateElement(obj);
    resizeColumToContents();
}

void AdministratorUsersWidget::slotRemoveUser(const QModelIndex &index)
{
#if KWIDGETSADDONS_VERSION >= QT_VERSION_CHECK(5, 100, 0)
    if (KMessageBox::ButtonCode::PrimaryAction
        == KMessageBox::questionTwoActions(this,
#else
    if (KMessageBox::Yes
        == KMessageBox::warningYesNo(this,
#endif
                                           i18n("Do you want to remove this user?"),
                                           i18n("Remove User"),
                                           KStandardGuiItem::remove(),
                                           KStandardGuiItem::cancel())) {
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
    mModel->setData(modelIndex, active, AdminUsersModel::ActiveUser);
}

void AdministratorUsersWidget::slotCustomContextMenuRequested(const QPoint &pos)
{
    QMenu menu(this);
    menu.addAction(QIcon::fromTheme(QStringLiteral("list-add")), i18n("Add..."), this, &AdministratorUsersWidget::slotAddUser);
    const QModelIndex index = mTreeView->indexAt(pos);
    if (index.isValid()) {
        const QModelIndex newModelIndex = mProxyModelModel->mapToSource(index);

        if (mRocketChatAccount->hasPermission(QStringLiteral("edit-other-user-info"))) {
            menu.addAction(QIcon::fromTheme(QStringLiteral("document-edit")), i18n("Modify..."), this, [this, newModelIndex]() {
                const QModelIndex modelIndex = mModel->index(newModelIndex.row(), AdminUsersModel::UserId);
                slotModifyUser(modelIndex);
            });
            menu.addSeparator();
        }
        if (mRocketChatAccount->hasPermission(QStringLiteral("edit-other-user-active-status"))) {
            const QModelIndex modelIndex = mModel->index(newModelIndex.row(), AdminUsersModel::ActiveUser);
            const bool activateUser = modelIndex.data().toBool();
            menu.addAction(activateUser ? i18n("Deactivate") : i18n("Active"), this, [this, newModelIndex, activateUser]() {
                slotActivateUser(newModelIndex, activateUser);
            });
            menu.addSeparator();
        }
        if (mRocketChatAccount->hasPermission(QStringLiteral("assign-admin-role"))) {
            const QModelIndex administratorIndex = mModel->index(newModelIndex.row(), AdminUsersModel::Administrator);
            const bool isAdministrator = administratorIndex.data().toBool();

            menu.addAction(isAdministrator ? i18n("Remove Admin") : i18n("Make Admin"), this, [this, newModelIndex, isAdministrator]() {
                const QModelIndex modelIndex = mModel->index(newModelIndex.row(), AdminUsersModel::UserId);
                slotChangeAdmin(modelIndex, !isAdministrator);
            });
        }
        if (mRocketChatAccount->hasPermission(QStringLiteral("edit-other-user-e2ee"))) {
            menu.addAction(i18n("Reset E2E Key"), this, [this, newModelIndex]() {
                const QModelIndex modelIndex = mModel->index(newModelIndex.row(), AdminUsersModel::UserId);
                slotResetE2EKey(modelIndex);
            });
        }
        if (mRocketChatAccount->hasPermission(QStringLiteral("edit-other-user-totp"))) {
            menu.addAction(i18n("Reset Totp"), this, [this, newModelIndex]() {
                const QModelIndex modelIndex = mModel->index(newModelIndex.row(), AdminUsersModel::UserId);
                slotResetTOTPKey(modelIndex);
            });
        }
        if (mRocketChatAccount->hasPermission(QStringLiteral("delete-user"))) {
            menu.addSeparator();
            menu.addAction(QIcon::fromTheme(QStringLiteral("list-remove")), i18n("Remove"), this, [this, newModelIndex]() {
                const QModelIndex i = mModel->index(newModelIndex.row(), AdminUsersModel::UserId);
                slotRemoveUser(i);
            });
        }
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

void AdministratorUsersWidget::slotChangeAdmin(const QModelIndex &index, bool adminStatus)
{
    const QModelIndex modelIndex = mModel->index(index.row(), AdminUsersModel::UserId);
    const QString userId = modelIndex.data().toString();
    mRocketChatAccount->ddp()->setAdminStatus(userId, adminStatus);
}

void AdministratorUsersWidget::slotResetE2EKey(const QModelIndex &index)
{
#if KWIDGETSADDONS_VERSION >= QT_VERSION_CHECK(5, 100, 0)
    if (KMessageBox::ButtonCode::PrimaryAction
        == KMessageBox::questionTwoActions(
            this,
#else
    if (KMessageBox::Yes
        == KMessageBox::warningYesNo(this,
#endif
            i18n("Reset the current E2E key will log out the user. When the user login again, Rocket.Chat "
                 "will generate a new key and restore the user access to any encrypted room that has one or more members "
                 "online. Due to the nature of the E2E encryption, Rocket.Chat will not be able to restore access to any encrypted "
                 "room that has no member online."),
            i18n("Reset E2E key"),
            KStandardGuiItem::reset(),
            KStandardGuiItem::cancel())) {
        QString password;
        const bool twoFactorAuthenticationEnforcePasswordFallback = mRocketChatAccount->twoFactorAuthenticationEnforcePasswordFallback();
        if (twoFactorAuthenticationEnforcePasswordFallback) {
            QPointer<ConfirmPasswordDialog> dialog(new ConfirmPasswordDialog(this));
            if (dialog->exec()) {
                password = dialog->password();
            }
            delete dialog;
        }

        auto job = new RocketChatRestApi::ResetE2EKeyJob(this);
        const QModelIndex modelIndex = mModel->index(index.row(), AdminUsersModel::UserId);
        const QString userId = modelIndex.data().toString();
        job->setResetUserId(userId);

        if (twoFactorAuthenticationEnforcePasswordFallback) {
            job->setAuthMethod(QStringLiteral("password"));
            job->setAuthCode(QString::fromLatin1(Utils::convertSha256Password(password)));
        }
        mRocketChatAccount->restApi()->initializeRestApiJob(job);

        const QModelIndex modelIndexUserName = mModel->index(index.row(), AdminUsersModel::UserName);
        const QString userName = modelIndexUserName.data().toString();
        connect(job, &RocketChatRestApi::ResetE2EKeyJob::resetE2EKeyDone, this, [this, userName]() {
            KMessageBox::information(this, i18n("E2E key for %1 has been reset.", userName), i18n("Reset E2E"));
        });
        if (!job->start()) {
            qCWarning(RUQOLAWIDGETS_LOG) << "Impossible to start ResetE2EKeyJob job";
        }
    }
}

void AdministratorUsersWidget::slotResetTOTPKey(const QModelIndex &index)
{
#if KWIDGETSADDONS_VERSION >= QT_VERSION_CHECK(5, 100, 0)
    if (KMessageBox::ButtonCode::PrimaryAction
        == KMessageBox::questionTwoActions(
            this,
#else
    if (KMessageBox::Yes
        == KMessageBox::warningYesNo(this,
#endif
            i18n("Reset the current Two Factor TOTP will log out the user. The user will be able to set the Two Factor again later."),
            i18n("Reset TOTP"),
            KStandardGuiItem::reset(),
            KStandardGuiItem::cancel())) {
        QString password;
        const bool twoFactorAuthenticationEnforcePasswordFallback = mRocketChatAccount->twoFactorAuthenticationEnforcePasswordFallback();
        if (twoFactorAuthenticationEnforcePasswordFallback) {
            QPointer<ConfirmPasswordDialog> dialog(new ConfirmPasswordDialog(this));
            if (dialog->exec()) {
                password = dialog->password();
            }
            delete dialog;
        }
        auto job = new RocketChatRestApi::ResetTOTPJob(this);
        const QModelIndex modelIndex = mModel->index(index.row(), AdminUsersModel::UserId);
        const QString userId = modelIndex.data().toString();
        job->setResetUserId(userId);

        if (twoFactorAuthenticationEnforcePasswordFallback) {
            job->setAuthMethod(QStringLiteral("password"));
            job->setAuthCode(QString::fromLatin1(Utils::convertSha256Password(password)));
        }
        mRocketChatAccount->restApi()->initializeRestApiJob(job);
        const QModelIndex modelIndexUserName = mModel->index(index.row(), AdminUsersModel::UserName);
        const QString userName = modelIndexUserName.data().toString();

        connect(job, &RocketChatRestApi::ResetTOTPJob::resetTOTPDone, this, [this, userName]() {
            KMessageBox::information(this, i18n("TOTP key for %1 has been reset", userName), i18n("Reset TOTP"));
        });
        if (!job->start()) {
            qCWarning(RUQOLAWIDGETS_LOG) << "Impossible to start ResetTOTPJob job";
        }
    }
}
