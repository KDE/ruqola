/*
   SPDX-FileCopyrightText: 2021-2023 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "usersinrolewidget.h"
#include "connection.h"
#include "dialogs/addusersinroomdialog.h"
#include "misc/searchwithdelaylineedit.h"
#include "model/searchtreebasefilterproxymodel.h"
#include "model/usersinrolemodel.h"
#include "rocketchataccount.h"
#include "role/addusertorolejob.h"
#include "role/getusersinrolejob.h"
#include "role/removeuserfromrolejob.h"
#include "ruqolawidgets_debug.h"
#include <KLocalizedString>
#include <KMessageBox>
#include <QLabel>
#include <QMenu>
#include <QPointer>
#include <QTreeView>

UsersInRoleWidget::UsersInRoleWidget(RocketChatAccount *account, QWidget *parent)
    : SearchTreeBaseWidget(account, parent)
{
    mModel = new UsersInRoleModel(this);
    mModel->setObjectName(QStringLiteral("mAdminUsersModel"));

    mProxyModelModel = new SearchTreeBaseFilterProxyModel(mModel, this);
    mProxyModelModel->setObjectName(QStringLiteral("mAdminUsersProxyModel"));
    mSearchLineEdit->setPlaceholderText(i18n("Search users..."));
    mTreeView->setModel(mProxyModelModel);
    hideColumns();
    connectModel();
}

UsersInRoleWidget::~UsersInRoleWidget() = default;

void UsersInRoleWidget::slotTextChanged(const QString &str)
{
    mProxyModelModel->setFilterString(str);
}

void UsersInRoleWidget::slotAddUser()
{
    QPointer<AddUsersInRoomDialog> dlg = new AddUsersInRoomDialog(mRocketChatAccount, this);
    dlg->setWindowTitle(i18nc("@title:window", "Add Users in Role"));
    if (dlg->exec()) {
        const auto users = dlg->userNames();
        for (const auto &user : users) {
            auto job = new RocketChatRestApi::AddUserToRoleJob(this);
            job->setRoleName(mRoleId);
            job->setUsername(user);
            mRocketChatAccount->restApi()->initializeRestApiJob(job);
            connect(job, &RocketChatRestApi::AddUserToRoleJob::addUsersToRoleDone, this, &UsersInRoleWidget::slotAddUsersToRoleDone);
            if (!job->start()) {
                qCWarning(RUQOLAWIDGETS_LOG) << "Impossible to start AddUserToRoleJob job";
            }
        }
    }
    delete dlg;
}

void UsersInRoleWidget::slotAddUsersToRoleDone(const QJsonObject &replyObject)
{
    // qDebug() << " replyObject " << replyObject;
    initialize(); // Reload list
}

void UsersInRoleWidget::slotRemoveUser(const QModelIndex &index)
{
    QModelIndex modelIndex = mTreeView->model()->index(index.row(), UsersInRoleModel::Name);
    if (KMessageBox::ButtonCode::PrimaryAction
        == KMessageBox::questionTwoActions(this,
                                           i18n("Do you want to remove this user \"%1\"?", modelIndex.data().toString()),
                                           i18n("Remove User"),
                                           KStandardGuiItem::remove(),
                                           KStandardGuiItem::cancel())) {
        auto job = new RocketChatRestApi::RemoveUserFromRoleJob(this);
        job->setRoleName(mRoleName);
        modelIndex = mTreeView->model()->index(index.row(), UsersInRoleModel::UserName);
        job->setUsername(modelIndex.data().toString());

        mRocketChatAccount->restApi()->initializeRestApiJob(job);
        connect(job, &RocketChatRestApi::RemoveUserFromRoleJob::removeUsersFromRoleDone, this, &UsersInRoleWidget::slotRemoveUsersFromRoleDone);
        if (!job->start()) {
            qCWarning(RUQOLAWIDGETS_LOG) << "Impossible to start RemoveUserFromRoleJob job";
        }
    }
}

void UsersInRoleWidget::slotRemoveUsersFromRoleDone(const QJsonObject &replyObject)
{
    initialize(); // Reload list
    // qDebug() << " replyObject " << replyObject;
}

void UsersInRoleWidget::slotCustomContextMenuRequested(const QPoint &pos)
{
    QMenu menu(this);
    menu.addAction(QIcon::fromTheme(QStringLiteral("list-add")), i18n("Add..."), this, &UsersInRoleWidget::slotAddUser);
    const QModelIndex index = mTreeView->indexAt(pos);
    if (index.isValid()) {
        menu.addSeparator();
        menu.addAction(QIcon::fromTheme(QStringLiteral("list-remove")), i18n("Remove"), this, [this, index]() {
            slotRemoveUser(index);
        });
    }
    menu.exec(mTreeView->viewport()->mapToGlobal(pos));
}

void UsersInRoleWidget::updateLabel()
{
    mLabelResultSearch->setText(mModel->total() == 0 ? i18n("No user found") : displayShowMessageInRoom());
}

QString UsersInRoleWidget::displayShowMessageInRoom() const
{
    QString displayMessageStr = i18np("%1 user (Total: %2)", "%1 users (Total: %2)", mModel->rowCount(), mModel->total());
    if (!mModel->hasFullList()) {
        displayMessageStr += clickableStr();
    }
    return displayMessageStr;
}

const QString &UsersInRoleWidget::roleName() const
{
    return mRoleName;
}

void UsersInRoleWidget::setRoleName(const QString &newRoleName)
{
    mRoleName = newRoleName;
}

const QString &UsersInRoleWidget::roleId() const
{
    return mRoleId;
}

void UsersInRoleWidget::setRoleId(const QString &newRoleId)
{
    if (newRoleId.isEmpty()) {
        qCWarning(RUQOLAWIDGETS_LOG) << " RoleId is empty !";
        return;
    }
    if (mRoleId != newRoleId) {
        mRoleId = newRoleId;
        initialize();
    }
}

void UsersInRoleWidget::slotLoadElements(int offset, int count, const QString &searchName)
{
    auto job = new RocketChatRestApi::GetUsersInRoleJob(this);
    job->setRoleId(mRoleId);
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
        connect(job, &RocketChatRestApi::GetUsersInRoleJob::getUsersInRoleDone, this, &UsersInRoleWidget::slotLoadMoreElementDone);
    } else {
        connect(job, &RocketChatRestApi::GetUsersInRoleJob::getUsersInRoleDone, this, &UsersInRoleWidget::slotSearchDone);
    }
    if (!job->start()) {
        qCWarning(RUQOLAWIDGETS_LOG) << "Impossible to start searchRoomUser job";
    }
}
