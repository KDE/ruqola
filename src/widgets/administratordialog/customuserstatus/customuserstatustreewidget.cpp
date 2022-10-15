/*
   SPDX-FileCopyrightText: 2020-2022 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "customuserstatustreewidget.h"
#include "administratorcustomuserstatuscreatedialog.h"
#include "rocketchataccount.h"
#include "ruqola.h"
#include "utils.h"
#include <KLocalizedString>
#include <KMessageBox>
#include <QHeaderView>
#include <QMenu>
#include <QPointer>
#include <kwidgetsaddons_version.h>

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
    const QVector<CustomUserStatus> customUserses = statuses.customUserses();
    for (const CustomUserStatus &status : customUserses) {
        auto item = new CustomUserStatusTreeWidgetItem(this);
        item->setUserStatus(status);
        item->setText(CustomUserStatusTreeWidget::Name, status.name());
        item->setText(CustomUserStatusTreeWidget::Presence, Utils::presenceStatusToString(status.statusType()));
    }
    setSortingEnabled(true);
    header()->setSortIndicatorShown(true);
    header()->setSectionsClickable(true);
    sortByColumn(0, Qt::AscendingOrder);
}

void CustomUserStatusTreeWidget::addClicked()
{
    QPointer<AdministratorCustomUserStatusCreateDialog> dlg = new AdministratorCustomUserStatusCreateDialog(this);
    if (dlg->exec()) {
        const AdministratorCustomUserStatusCreateWidget::UserStatusInfo info = dlg->userStatusInfo();
        RocketChatRestApi::CustomUserStatusCreateJob::StatusCreateInfo statusCreateInfo;
        statusCreateInfo.name = info.name;
        statusCreateInfo.statusType = Utils::presenceStatusToString(info.statusType);
        mRocketChatAccount->createCustomUserStatus(statusCreateInfo);
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
    dlg->setUserStatusInfo(userStatusinfo);
    if (dlg->exec()) {
        const AdministratorCustomUserStatusCreateWidget::UserStatusInfo info = dlg->userStatusInfo();
        RocketChatRestApi::CustomUserStatusUpdateJob::StatusUpdateInfo statusUpdateInfo;
        statusUpdateInfo.name = info.name;
        statusUpdateInfo.statusType = Utils::presenceStatusToString(info.statusType);
        statusUpdateInfo.identifier = userStatus.identifier();
        mRocketChatAccount->updateCustomUserStatus(statusUpdateInfo);
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
#if KWIDGETSADDONS_VERSION >= QT_VERSION_CHECK(5, 100, 0)
    if (KMessageBox::ButtonCode::PrimaryAction
        == KMessageBox::questionTwoActions(this,
#else
    if (KMessageBox::Yes
        == KMessageBox::questionYesNo(this,
#endif
                                           i18n("Do you want to remove \"%1\"?", userStatus.name()),
                                           i18nc("@title", "Remove Custom User Status"),
                                           KStandardGuiItem::remove(),
                                           KStandardGuiItem::cancel())) {
        mRocketChatAccount->removeCustomUserStatus(userStatus.identifier());
    }
}

void CustomUserStatusTreeWidget::slotCustomContextMenuRequested(const QPoint &pos)
{
    QMenu menu(this);
    menu.addAction(QIcon::fromTheme(QStringLiteral("list-add")), i18n("Add..."), this, &CustomUserStatusTreeWidget::addClicked);
    QTreeWidgetItem *item = itemAt(pos);
    if (item) {
        menu.addAction(QIcon::fromTheme(QStringLiteral("document-edit")), i18n("Modify..."), this, &CustomUserStatusTreeWidget::editClicked);
        menu.addSeparator();
        menu.addAction(QIcon::fromTheme(QStringLiteral("list-remove")), i18n("Remove"), this, &CustomUserStatusTreeWidget::removeClicked);
    }
    menu.exec(viewport()->mapToGlobal(pos));
}
