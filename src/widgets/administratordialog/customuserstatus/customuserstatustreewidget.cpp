/*
   SPDX-FileCopyrightText: 2020-2026 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "customuserstatustreewidget.h"

#include "administratorcustomuserstatuscreatedialog.h"
#include "connection.h"
#include "custom/customuserstatuscreatejob.h"
#include "custom/customuserstatusdeletejob.h"
#include "custom/customuserstatusupdatejob.h"
#include "rocketchataccount.h"
#include "ruqolawidgets_debug.h"
#include "utils.h"
#include <KLocalizedString>
#include <KMessageBox>
#include <QHeaderView>
#include <QMenu>
#include <QPointer>

using namespace Qt::Literals::StringLiterals;
CustomUserStatusTreeWidgetItem::CustomUserStatusTreeWidgetItem(QTreeWidget *parent)
    : QTreeWidgetItem(parent)
{
}

CustomUserStatusTreeWidgetItem::~CustomUserStatusTreeWidgetItem() = default;

CustomUserStatus CustomUserStatusTreeWidgetItem::userStatus() const
{
    return mUserStatus;
}

void CustomUserStatusTreeWidgetItem::setUserStatus(const CustomUserStatus &userStatus)
{
    mUserStatus = userStatus;
}

CustomUserStatusTreeWidget::CustomUserStatusTreeWidget(RocketChatAccount *account, QWidget *parent)
    : QTreeWidget(parent)
    , mRocketChatAccount(account)
{
    setColumnCount(2);
    setHeaderLabels({i18n("Name"), i18n("Presence")});
    setAlternatingRowColors(true);
    header()->setSectionsMovable(false);
    header()->setSectionResizeMode(QHeaderView::ResizeToContents);
    setSelectionMode(SingleSelection);
    setContextMenuPolicy(Qt::CustomContextMenu);
    setRootIsDecorated(false);
    connect(this, &CustomUserStatusTreeWidget::customContextMenuRequested, this, &CustomUserStatusTreeWidget::slotCustomContextMenuRequested);
    connect(mRocketChatAccount, &RocketChatAccount::customUserStatusChanged, this, &CustomUserStatusTreeWidget::initialize);
    connect(this, &CustomUserStatusTreeWidget::itemDoubleClicked, this, &CustomUserStatusTreeWidget::editClicked);
}

CustomUserStatusTreeWidget::~CustomUserStatusTreeWidget() = default;

void CustomUserStatusTreeWidget::initialize()
{
    clear();
    const CustomUserStatuses statuses = mRocketChatAccount->customUserStatuses();
    const QList<CustomUserStatus> customUserses = statuses.customUserStatusList();
    for (const CustomUserStatus &status : customUserses) {
        auto item = new CustomUserStatusTreeWidgetItem(this);
        item->setUserStatus(status);
        item->setText(CustomUserStatusTreeWidget::Name, status.name());
        item->setText(CustomUserStatusTreeWidget::Presence, Utils::i18nFromPresenceStatus(status.statusType()));
    }
    setSortingEnabled(true);
    header()->setSortIndicatorShown(true);
    header()->setSectionsClickable(true);
    sortByColumn(0, Qt::AscendingOrder);
}

void CustomUserStatusTreeWidget::addClicked()
{
    QPointer<AdministratorCustomUserStatusCreateDialog> dlg = new AdministratorCustomUserStatusCreateDialog(this);
    const CustomUserStatuses statuses = mRocketChatAccount->customUserStatuses();
    const QList<CustomUserStatus> customUserses = statuses.customUserStatusList();
    QStringList names;
    names.reserve(customUserses.count());
    for (const CustomUserStatus &status : customUserses) {
        names << status.name();
    }
    dlg->setExistingCustomUserNames(names);
    if (dlg->exec()) {
        const AdministratorCustomUserStatusCreateWidget::UserStatusInfo info = dlg->userStatusInfo();
        RocketChatRestApi::CustomUserStatusCreateJob::StatusCreateInfo statusCreateInfo;
        statusCreateInfo.name = info.name;
        statusCreateInfo.statusType = Utils::presenceStatusToString(info.statusType);
        auto job = new RocketChatRestApi::CustomUserStatusCreateJob(this);
        job->setStatusCreateInfo(statusCreateInfo);
        mRocketChatAccount->restApi()->initializeRestApiJob(job);
        if (!job->start()) {
            qCDebug(RUQOLAWIDGETS_LOG) << "Impossible to start CustomUserStatusCreateJob";
        }
    }
    delete dlg;
}

void CustomUserStatusTreeWidget::editClicked()
{
    if (!currentItem()) {
        return;
    }

    QPointer<AdministratorCustomUserStatusCreateDialog> dlg = new AdministratorCustomUserStatusCreateDialog(this);
    auto customUserStatusItem = static_cast<CustomUserStatusTreeWidgetItem *>(currentItem());
    const CustomUserStatus userStatus = customUserStatusItem->userStatus();
    AdministratorCustomUserStatusCreateWidget::UserStatusInfo userStatusinfo;
    userStatusinfo.name = userStatus.name();
    userStatusinfo.statusType = userStatus.statusType();

    const CustomUserStatuses statuses = mRocketChatAccount->customUserStatuses();
    const QList<CustomUserStatus> customUserses = statuses.customUserStatusList();
    QStringList names;
    for (const CustomUserStatus &status : customUserses) {
        if (userStatus.name() != status.name()) {
            names << status.name();
        }
    }
    dlg->setExistingCustomUserNames(names);

    dlg->setUserStatusInfo(userStatusinfo);
    if (dlg->exec()) {
        const AdministratorCustomUserStatusCreateWidget::UserStatusInfo info = dlg->userStatusInfo();
        RocketChatRestApi::CustomUserStatusUpdateJob::StatusUpdateInfo statusUpdateInfo;
        statusUpdateInfo.name = info.name;
        statusUpdateInfo.statusType = Utils::presenceStatusToString(info.statusType);
        statusUpdateInfo.identifier = userStatus.identifier();

        auto job = new RocketChatRestApi::CustomUserStatusUpdateJob(this);
        job->setStatusUpdateInfo(statusUpdateInfo);
        mRocketChatAccount->restApi()->initializeRestApiJob(job);
        if (!job->start()) {
            qCDebug(RUQOLAWIDGETS_LOG) << "Impossible to start CustomUserStatusUpdateJob";
        }
    }
    delete dlg;
}

void CustomUserStatusTreeWidget::removeClicked()
{
    if (!currentItem()) {
        return;
    }
    auto customUserStatusItem = static_cast<CustomUserStatusTreeWidgetItem *>(currentItem());
    const CustomUserStatus userStatus = customUserStatusItem->userStatus();
    if (KMessageBox::ButtonCode::PrimaryAction
        == KMessageBox::questionTwoActions(this,
                                           i18n("Do you want to remove \"%1\"?", userStatus.name()),
                                           i18nc("@title", "Remove Custom User Status"),
                                           KStandardGuiItem::remove(),
                                           KStandardGuiItem::cancel())) {
        auto job = new RocketChatRestApi::CustomUserStatusDeleteJob(this);
        job->setCustomUserStatusId(userStatus.identifier());
        mRocketChatAccount->restApi()->initializeRestApiJob(job);
        if (!job->start()) {
            qCDebug(RUQOLAWIDGETS_LOG) << "Impossible to start CustomUserStatusDeleteJob";
        }
    }
}

void CustomUserStatusTreeWidget::slotCustomContextMenuRequested(const QPoint &pos)
{
    QMenu menu(this);
    menu.addAction(QIcon::fromTheme(u"list-add"_s), i18nc("@action", "Add…"), this, &CustomUserStatusTreeWidget::addClicked);
    QTreeWidgetItem *item = itemAt(pos);
    if (item) {
        menu.addAction(QIcon::fromTheme(u"document-edit"_s), i18nc("@action", "Modify…"), this, &CustomUserStatusTreeWidget::editClicked);
        menu.addSeparator();
        menu.addAction(QIcon::fromTheme(u"list-remove"_s), i18nc("@action", "Remove"), this, &CustomUserStatusTreeWidget::removeClicked);
    }
    menu.exec(viewport()->mapToGlobal(pos));
}

#include "moc_customuserstatustreewidget.cpp"
