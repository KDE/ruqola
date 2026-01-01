/*
   SPDX-FileCopyrightText: 2021-2026 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "administratoruserswidget.h"
#include "administratoradduserdialog.h"
#include "administratorinviteusersdialog.h"
#include "administratoruserspendingactiondelegate.h"
#include "connection.h"
#include "ddpapi/ddpclient.h"
#include "dialogs/confirmpassworddialog.h"
#include "invite/sendinvitationemailjob.h"
#include "misc/rolescombobox.h"
#include "misc/searchwithdelaylineedit.h"
#include "model/adminusersallmodel.h"
#include "model/adminuserspendingmodel.h"
#include "model/adminusersstatusmodel.h"
#include "model/searchtreebasefilterproxymodel.h"
#include "rocketchataccount.h"
#include "ruqolaserverconfig.h"
#include "ruqolawidgets_debug.h"
#include "users/deleteuserjob.h"
#include "users/resete2ekeyjob.h"
#include "users/resettotpjob.h"
#include "users/setuseractivestatusjob.h"
#include "users/userinfojob.h"
#include "users/userscreatejob.h"
#include "users/userslistbystatusjob.h"
#include "users/userssendwelcomeemailjob.h"
#include "users/usersupdatejob.h"
#include "utils.h"
#include <KLocalizedString>
#include <KMessageBox>
#include <QHBoxLayout>
#include <QLabel>
#include <QMenu>
#include <QPointer>
#include <QTreeView>
using namespace Qt::Literals::StringLiterals;

AdministratorUsersWidget::AdministratorUsersWidget(AdministratorUsersWidget::UsersType type, RocketChatAccount *account, QWidget *parent)
    : SearchTreeBaseWidget(account, parent)
    , mRolesComboBox(new RolesComboBox(this))
    , mUserType(type)
{
    AdminUsersBaseModel *adminUsersModel = nullptr;
    switch (type) {
    case AdministratorUsersWidget::UsersType::Pending: {
        adminUsersModel = new AdminUsersPendingModel(this);
        auto delegate = new AdministratorUsersPendingActionDelegate(this);
        connect(delegate, &AdministratorUsersPendingActionDelegate::pendingActionActivated, this, [this](const QModelIndex &index) {
            const bool isActive = index.model()->index(index.row(), AdminUsersPendingModel::ActiveUser).data().toBool();
            if (isActive) {
                resendWelcomeEmail(index);
            } else {
                activateUser(index);
            }
        });
        mTreeView->setItemDelegateForColumn(AdminUsersPendingModel::PendingActionButton, delegate);
        break;
    }
    case UsersType::All:
        adminUsersModel = new AdminUsersAllModel(this);
        break;
    case UsersType::Deactivated:
    case UsersType::Activate:
        adminUsersModel = new AdminUsersStatusModel(this);
        break;
    }

    adminUsersModel->setObjectName("mAdminUsersModel"_L1);
    if (account) {
        adminUsersModel->setRoles(account->roleInfo());
    }
    mModel = adminUsersModel;

    mProxyModelModel = new SearchTreeBaseFilterProxyModel(mModel, this);
    mProxyModelModel->setObjectName("mAdminUsersProxyModel"_L1);
    mSearchLineEdit->setPlaceholderText(i18nc("@info:placeholder", "Search users…"));
    mTreeView->setModel(mProxyModelModel);
    if (type == AdministratorUsersWidget::UsersType::Pending) {
        auto funct = [this](const QModelIndex &left, const QModelIndex &right, bool &useDefaultLessThan) {
            const int leftColumn{left.column()};
            if (leftColumn == AdminUsersPendingModel::PendingActionButton) {
                const QString leftMessageModelIndex = mModel->index(left.row(), AdminUsersPendingModel::PendingActionInfo).data().toString();
                const QString rightMessageModelIndex = mModel->index(right.row(), AdminUsersPendingModel::PendingActionInfo).data().toString();
                useDefaultLessThan = false;
                return leftMessageModelIndex < rightMessageModelIndex;
            }
            useDefaultLessThan = true;
            return false;
        };
        mProxyModelModel->setSortFunction(funct);
    }

    mRolesComboBox->setObjectName("mRolesComboBox"_L1);
    mSearchLineLayout->addWidget(mRolesComboBox);
    if (account) {
        mRolesComboBox->setRolesInfo(account->roleInfo());
    }

    connect(mTreeView, &QTreeView::doubleClicked, this, &AdministratorUsersWidget::slotModifyDoubleClickUser);
    connect(mRolesComboBox, &RolesComboBox::rolesChanged, this, &AdministratorUsersWidget::slotRolesChanged);
    hideColumns();
    connectModel();
}

AdministratorUsersWidget::~AdministratorUsersWidget() = default;

void AdministratorUsersWidget::slotTextChanged(const QString &str)
{
    mProxyModelModel->setFilterString(str);
}

void AdministratorUsersWidget::slotRolesChanged()
{
    refreshLoadElements();
}

void AdministratorUsersWidget::slotInviteUsers()
{
    QPointer<AdministratorInviteUsersDialog> dlg = new AdministratorInviteUsersDialog(this);
    if (dlg->exec()) {
        const QStringList emails = dlg->emails();
        if (!emails.isEmpty()) {
            auto job = new RocketChatRestApi::SendInvitationEmailJob(this);
            job->setEmails(emails);
            mRocketChatAccount->restApi()->initializeRestApiJob(job);
            connect(job, &RocketChatRestApi::SendInvitationEmailJob::sendInvitationEmailsDone, this, []() {
                qCDebug(RUQOLAWIDGETS_LOG) << " Emails send";
            });
            if (!job->start()) {
                qCWarning(RUQOLAWIDGETS_LOG) << "Impossible to start SendInvitationEmailJob job";
            }
        }
    }
    delete dlg;
}

void AdministratorUsersWidget::slotAddUser()
{
    QPointer<AdministratorAddUserDialog> dlg = new AdministratorAddUserDialog(mRocketChatAccount, this);
    dlg->setRolesInfo(mRocketChatAccount->roleInfo());
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
        const QModelIndex modelIndex = mModel->index(newModelIndex.row(), AdminUsersAllModel::UserId);
        slotModifyUser(modelIndex);
    }
}

void AdministratorUsersWidget::slotModifyUser(const QModelIndex &index)
{
    auto userJob = new RocketChatRestApi::UserInfoJob(this);
    mRocketChatAccount->restApi()->initializeRestApiJob(userJob);
    RocketChatRestApi::UserBaseJob::UserInfo info;
    info.userInfoType = RocketChatRestApi::UserBaseJob::UserInfoType::UserId;
    const QByteArray userId = index.data().toByteArray();
    info.userIdentifier = QString::fromLatin1(userId);
    userJob->setUserInfo(info);
    connect(userJob, &RocketChatRestApi::UserInfoJob::userInfoDone, this, &AdministratorUsersWidget::slotUserInfoDone);
    if (!userJob->start()) {
        qCDebug(RUQOLAWIDGETS_LOG) << "Impossible to start UserInfoJob";
    }
}

void AdministratorUsersWidget::slotUserInfoDone(const QJsonObject &obj)
{
    QPointer<AdministratorAddUserDialog> dlg = new AdministratorAddUserDialog(mRocketChatAccount, this);
    dlg->setRolesInfo(mRocketChatAccount->roleInfo());
    User user;
    user.parseUserRestApi(obj["user"_L1].toObject(), mRocketChatAccount->roleInfo());
    dlg->setUser(user);
    if (dlg->exec()) {
        QString password;
        const bool twoFactorAuthenticationEnforcePasswordFallback = mRocketChatAccount->ruqolaServerConfig()->twoFactorAuthenticationEnforcePasswordFallback();
        if (twoFactorAuthenticationEnforcePasswordFallback) {
            QPointer<ConfirmPasswordDialog> dialog(new ConfirmPasswordDialog(this));
            bool accepted = false;
            if (dialog->exec()) {
                password = dialog->password();
                accepted = true;
            }
            delete dialog;
            if (!accepted) {
                return;
            }
        }
        RocketChatRestApi::UpdateUserInfo info = dlg->updateInfo();
        info.mUserId = QString::fromLatin1(user.userId());
        auto job = new RocketChatRestApi::UsersUpdateJob(this);
        job->setUpdateInfo(info);
        if (twoFactorAuthenticationEnforcePasswordFallback) {
            job->setAuthMethod(u"password"_s);
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
    if (KMessageBox::ButtonCode::PrimaryAction
        == KMessageBox::questionTwoActions(this,
                                           i18n("Do you want to remove this user?"),
                                           i18nc("@title:window", "Remove User"),
                                           KStandardGuiItem::remove(),
                                           KStandardGuiItem::cancel())) {
        auto job = new RocketChatRestApi::DeleteUserJob(this);
        RocketChatRestApi::UserBaseJob::UserInfo info;
        info.userInfoType = RocketChatRestApi::UserBaseJob::UserInfoType::UserId;
        const QByteArray userId = index.data().toByteArray();
        info.userIdentifier = QString::fromLatin1(userId);
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

void AdministratorUsersWidget::slotDeleteUserDone(const QByteArray &userId)
{
    mModel->removeElement(userId);
}

void AdministratorUsersWidget::slotActivateUser(const QModelIndex &index, bool activateUser)
{
    auto job = new RocketChatRestApi::SetUserActiveStatusJob(this);
    const QModelIndex modelIndex = mModel->index(index.row(), AdminUsersAllModel::UserId);
    const QByteArray userId = modelIndex.data().toByteArray();
    job->setActivate(!activateUser);
    job->setActivateUserId(userId);
    mRocketChatAccount->restApi()->initializeRestApiJob(job);
    connect(job, &RocketChatRestApi::SetUserActiveStatusJob::setUserActiveStatusDone, this, [this, modelIndex](const QJsonObject &replyObject) {
        slotSetUserActiveStatus(replyObject, modelIndex);
        refreshLoadElements();
    });
    if (!job->start()) {
        qCWarning(RUQOLAWIDGETS_LOG) << "Impossible to start SetUserActiveStatusJob job";
    }
}

void AdministratorUsersWidget::slotSetUserActiveStatus(const QJsonObject &replyObject, const QModelIndex &modelIndex)
{
    const QJsonObject userObj = replyObject["user"_L1].toObject();
    const bool active = userObj["active"_L1].toBool();
    mModel->setData(modelIndex, active, AdminUsersAllModel::ActiveUser);
}

void AdministratorUsersWidget::slotCustomContextMenuRequested(const QPoint &pos)
{
    QMenu menu(this);
    if (mRocketChatAccount->hasPermission(u"bulk-register-user"_s)) {
        menu.addAction(QIcon::fromTheme(u"list-add"_s), i18nc("@action", "Invite…"), this, &AdministratorUsersWidget::slotInviteUsers);
    }
    if (mRocketChatAccount->hasPermission(u"create-user"_s)) {
        if (!menu.isEmpty()) {
            menu.addSeparator();
        }
        menu.addAction(QIcon::fromTheme(u"list-add"_s), i18nc("@action", "Add…"), this, &AdministratorUsersWidget::slotAddUser);
    }
    const QModelIndex index = mTreeView->indexAt(pos);
    if (index.isValid()) {
        const QModelIndex newModelIndex = mProxyModelModel->mapToSource(index);

        if (mRocketChatAccount->hasPermission(u"edit-other-user-info"_s)) {
            menu.addAction(QIcon::fromTheme(u"document-edit"_s), i18nc("@action", "Modify…"), this, [this, newModelIndex]() {
                const QModelIndex modelIndex = mModel->index(newModelIndex.row(), AdminUsersAllModel::UserId);
                slotModifyUser(modelIndex);
            });
            menu.addSeparator();
        }
        if (mRocketChatAccount->hasPermission(u"edit-other-user-active-status"_s)) {
            const QModelIndex modelIndex = mModel->index(newModelIndex.row(), AdminUsersAllModel::ActiveUser);
            const bool activateUser = modelIndex.data().toBool();
            menu.addAction(activateUser ? i18nc("@action", "Deactivate") : i18nc("@action", "Active"), this, [this, newModelIndex, activateUser]() {
                slotActivateUser(newModelIndex, activateUser);
            });
            menu.addSeparator();
        }
        if (mRocketChatAccount->hasPermission(u"assign-admin-role"_s)) {
            const QModelIndex administratorIndex = mModel->index(newModelIndex.row(), AdminUsersAllModel::Administrator);
            const bool isAdministrator = administratorIndex.data().toBool();

            menu.addAction(isAdministrator ? i18nc("@action", "Remove Admin") : i18nc("@action", "Make Admin"), this, [this, newModelIndex, isAdministrator]() {
                const QModelIndex modelIndex = mModel->index(newModelIndex.row(), AdminUsersAllModel::UserId);
                slotChangeAdmin(modelIndex, !isAdministrator);
            });
        }
        if (mRocketChatAccount->hasPermission(u"edit-other-user-e2ee"_s)) {
            menu.addAction(i18nc("@action", "Reset E2E Key"), this, [this, newModelIndex]() {
                const QModelIndex modelIndex = mModel->index(newModelIndex.row(), AdminUsersAllModel::UserId);
                slotResetE2EKey(modelIndex);
            });
        }
        if (mRocketChatAccount->hasPermission(u"edit-other-user-totp"_s) && mRocketChatAccount->ruqolaServerConfig()->twoFactorAuthenticationEnabled()) {
            menu.addAction(i18nc("@action", "Reset Totp"), this, [this, newModelIndex]() {
                const QModelIndex modelIndex = mModel->index(newModelIndex.row(), AdminUsersAllModel::UserId);
                slotResetTOTPKey(modelIndex);
            });
        }
        if (mRocketChatAccount->hasPermission(u"delete-user"_s)) {
            menu.addSeparator();
            menu.addAction(QIcon::fromTheme(u"list-remove"_s), i18nc("@action", "Remove"), this, [this, newModelIndex]() {
                const QModelIndex i = mModel->index(newModelIndex.row(), AdminUsersAllModel::UserId);
                slotRemoveUser(i);
            });
        }
    }
    if (!menu.isEmpty()) {
        menu.exec(mTreeView->viewport()->mapToGlobal(pos));
    }
}

void AdministratorUsersWidget::updateLabel()
{
    mLabelResultSearch->setText(mModel->total() == 0 ? i18n("No user found") : displayShowUsers());
}

QString AdministratorUsersWidget::displayShowUsers() const
{
    QString displayMessageStr = i18np("%1 user (Total: %2)", "%1 users (Total: %2)", mModel->rowCount(), mModel->total());
    if (!mModel->hasFullList()) {
        displayMessageStr += clickableStr();
    }
    return displayMessageStr;
}

void AdministratorUsersWidget::slotLoadElements(int offset, int count, const QString &searchName)
{
    auto job = new RocketChatRestApi::UsersListByStatusJob(this);
    RocketChatRestApi::UsersListByStatusJob::UsersListByStatusJobInfo info;
    const QStringList roles = mRolesComboBox->roles();
    if (!roles.isEmpty()) {
        info.roles = roles;
    }
    if (!searchName.isEmpty()) {
        info.searchName = searchName;
    }

    switch (mUserType) {
    case UsersType::Pending: {
        info.hasLoggedIn = RocketChatRestApi::UsersListByStatusJob::LoggedStatus::NotLogged;
        info.type = RocketChatRestApi::UsersListByStatusJob::StatusType::User;
        break;
    }
    case UsersType::Deactivated: {
        info.hasLoggedIn = RocketChatRestApi::UsersListByStatusJob::LoggedStatus::Logged;
        info.type = RocketChatRestApi::UsersListByStatusJob::StatusType::User;
        info.status = RocketChatRestApi::UsersListByStatusJob::Status::Deactivated;
        break;
    }
    case UsersType::Activate: {
        // TODO fix me
        info.hasLoggedIn = RocketChatRestApi::UsersListByStatusJob::LoggedStatus::Logged;
        info.type = RocketChatRestApi::UsersListByStatusJob::StatusType::User;
        info.status = RocketChatRestApi::UsersListByStatusJob::Status::Activated;
        break;
    }
    case UsersType::All:
        break;
    }

    job->setUsersListByStatusJobInfo(info);
    RocketChatRestApi::QueryParameters parameters;
    QMap<QString, RocketChatRestApi::QueryParameters::SortOrder> map;
    map.insert(u"name"_s, RocketChatRestApi::QueryParameters::SortOrder::Ascendant);
    parameters.setSorting(map);
    if (offset != -1) {
        parameters.setOffset(offset);
    }
    if (count != -1) {
        parameters.setCount(count);
    }
    job->setQueryParameters(parameters);

    mRocketChatAccount->restApi()->initializeRestApiJob(job);
    if (offset != -1) {
        connect(job, &RocketChatRestApi::UsersListByStatusJob::usersListByStatusDone, this, &AdministratorUsersWidget::slotLoadMoreElementDone);
    } else {
        connect(job, &RocketChatRestApi::UsersListByStatusJob::usersListByStatusDone, this, &AdministratorUsersWidget::slotSearchDone);
    }
    if (!job->start()) {
        qCWarning(RUQOLAWIDGETS_LOG) << "Impossible to start UsersListByStatusJob job";
    }
}

void AdministratorUsersWidget::slotChangeAdmin(const QModelIndex &index, bool adminStatus)
{
    const QModelIndex modelIndex = mModel->index(index.row(), AdminUsersAllModel::UserId);
    const QByteArray userId = modelIndex.data().toByteArray();
    mRocketChatAccount->ddp()->setAdminStatus(userId, adminStatus);
}

void AdministratorUsersWidget::slotResetE2EKey(const QModelIndex &index)
{
    if (KMessageBox::ButtonCode::PrimaryAction
        == KMessageBox::questionTwoActions(
            this,
            i18n("Reset the current E2E key will log out the user. When the user login again, Rocket.Chat "
                 "will generate a new key and restore the user access to any encrypted room that has one or more members "
                 "online. Due to the nature of the E2E encryption, Rocket.Chat will not be able to restore access to any encrypted "
                 "room that has no member online."),
            i18nc("@title:window", "Reset E2E key"),
            KStandardGuiItem::reset(),
            KStandardGuiItem::cancel())) {
        QString password;
        const bool twoFactorAuthenticationEnforcePasswordFallback = mRocketChatAccount->ruqolaServerConfig()->twoFactorAuthenticationEnforcePasswordFallback();
        if (twoFactorAuthenticationEnforcePasswordFallback) {
            QPointer<ConfirmPasswordDialog> dialog(new ConfirmPasswordDialog(this));
            bool accepted = false;
            if (dialog->exec()) {
                password = dialog->password();
                accepted = true;
            }
            delete dialog;
            if (!accepted) {
                return;
            }
        }

        auto job = new RocketChatRestApi::ResetE2EKeyJob(this);
        const QModelIndex modelIndex = mModel->index(index.row(), AdminUsersAllModel::UserId);
        const QByteArray userId = modelIndex.data().toByteArray();
        job->setResetUserId(userId);

        if (twoFactorAuthenticationEnforcePasswordFallback) {
            job->setAuthMethod(u"password"_s);
            job->setAuthCode(QString::fromLatin1(Utils::convertSha256Password(password)));
        }
        mRocketChatAccount->restApi()->initializeRestApiJob(job);

        const QModelIndex modelIndexUserName = mModel->index(index.row(), AdminUsersAllModel::UserName);
        const QString userName = modelIndexUserName.data().toString();
        connect(job, &RocketChatRestApi::ResetE2EKeyJob::resetE2EKeyDone, this, [this, userName]() {
            KMessageBox::information(this, i18n("E2E key for %1 has been reset.", userName), i18nc("@title:window", "Reset E2E"));
        });
        if (!job->start()) {
            qCWarning(RUQOLAWIDGETS_LOG) << "Impossible to start ResetE2EKeyJob job";
        }
    }
}

void AdministratorUsersWidget::slotResetTOTPKey(const QModelIndex &index)
{
    if (KMessageBox::ButtonCode::PrimaryAction
        == KMessageBox::questionTwoActions(
            this,
            i18n("Reset the current Two Factor TOTP will log out the user. The user will be able to set the Two Factor again later."),
            i18nc("@title:window", "Reset TOTP"),
            KStandardGuiItem::reset(),
            KStandardGuiItem::cancel())) {
        QString password;
        const bool twoFactorAuthenticationEnforcePasswordFallback = mRocketChatAccount->ruqolaServerConfig()->twoFactorAuthenticationEnforcePasswordFallback();
        if (twoFactorAuthenticationEnforcePasswordFallback) {
            QPointer<ConfirmPasswordDialog> dialog(new ConfirmPasswordDialog(this));
            bool accepted = false;
            if (dialog->exec()) {
                password = dialog->password();
                accepted = true;
            }
            delete dialog;
            if (!accepted) {
                return;
            }
        }
        auto job = new RocketChatRestApi::ResetTOTPJob(this);
        const QModelIndex modelIndex = mModel->index(index.row(), AdminUsersAllModel::UserId);
        const QByteArray userId = modelIndex.data().toByteArray();
        job->setResetUserId(userId);

        if (twoFactorAuthenticationEnforcePasswordFallback) {
            job->setAuthMethod(u"password"_s);
            job->setAuthCode(QString::fromLatin1(Utils::convertSha256Password(password)));
        }
        mRocketChatAccount->restApi()->initializeRestApiJob(job);
        const QModelIndex modelIndexUserName = mModel->index(index.row(), AdminUsersAllModel::UserName);
        const QString userName = modelIndexUserName.data().toString();

        connect(job, &RocketChatRestApi::ResetTOTPJob::resetTOTPDone, this, [this, userName]() {
            KMessageBox::information(this, i18n("TOTP key for %1 has been reset", userName), i18nc("@title:window", "Reset TOTP"));
        });
        if (!job->start()) {
            qCWarning(RUQOLAWIDGETS_LOG) << "Impossible to start ResetTOTPJob job";
        }
    }
}

void AdministratorUsersWidget::resendWelcomeEmail(const QModelIndex &index)
{
    const QString email = mModel->index(index.row(), AdminUsersAllModel::Email).data().toString();
    auto job = new RocketChatRestApi::UsersSendWelcomeEmailJob(this);
    job->setEmail(email);
    mRocketChatAccount->restApi()->initializeRestApiJob(job);
    connect(job, &RocketChatRestApi::UsersSendWelcomeEmailJob::sendWelcomeEmailDone, this, []() {
        qDebug(RUQOLAWIDGETS_LOG) << " email sent !!!";
    });
    if (!job->start()) {
        qCWarning(RUQOLAWIDGETS_LOG) << "Impossible to start UsersSendWelcomeEmailJob job";
    }
}

void AdministratorUsersWidget::activateUser(const QModelIndex &index)
{
    // Use false here as slotActivateUser revert activate value => we need to use "false" to activate it :)
    slotActivateUser(index, false);
}

#include "moc_administratoruserswidget.cpp"
