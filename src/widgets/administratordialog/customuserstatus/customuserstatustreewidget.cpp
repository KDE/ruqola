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

#include "customuserstatustreewidget.h"
#include "administratorcustomuserstatuscreatedialog.h"
#include "ruqola.h"
#include "utils.h"
#include "rocketchataccount.h"
#include <KLocalizedString>
#include <KMessageBox>
#include <QHeaderView>
#include <QMenu>
#include <QPointer>

CustomUserStatusTreeWidgetItem::CustomUserStatusTreeWidgetItem(QTreeWidget *parent)
    : QTreeWidgetItem(parent)
{
}

CustomUserStatusTreeWidgetItem::~CustomUserStatusTreeWidgetItem()
{
}

CustomUserStatus CustomUserStatusTreeWidgetItem::userStatus() const
{
    return mUserStatus;
}

void CustomUserStatusTreeWidgetItem::setUserStatus(const CustomUserStatus &userStatus)
{
    mUserStatus = userStatus;
}

CustomUserStatusTreeWidget::CustomUserStatusTreeWidget(QWidget *parent)
    : QTreeWidget(parent)
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
    initialize();
    connect(Ruqola::self()->rocketChatAccount(), &RocketChatAccount::customUserStatusChanged, this, &CustomUserStatusTreeWidget::initialize);
}

CustomUserStatusTreeWidget::~CustomUserStatusTreeWidget()
{
}

void CustomUserStatusTreeWidget::initialize()
{
    clear();
    const CustomUserStatuses statuses = Ruqola::self()->rocketChatAccount()->customUserStatuses();
    const QVector<CustomUserStatus> customUserses = statuses.customUserses();
    for (const CustomUserStatus &status : customUserses) {
        auto *item = new CustomUserStatusTreeWidgetItem(this);
        item->setUserStatus(status);
        item->setText(CustomUserStatusTreeWidget::Name, status.name());
        item->setText(CustomUserStatusTreeWidget::Presence, Utils::presenceStatusToString(status.statusType()));
    }
}

void CustomUserStatusTreeWidget::addClicked()
{
    QPointer<AdministratorCustomUserStatusCreateDialog> dlg = new AdministratorCustomUserStatusCreateDialog(this);
    if (dlg->exec()) {
        const AdministratorCustomUserStatusCreateWidget::UserStatusInfo info = dlg->userStatusInfo();
        RocketChatRestApi::CustomUserStatusCreateJob::StatusCreateInfo statusCreateInfo;
        statusCreateInfo.name = info.name;
        statusCreateInfo.statusType = Utils::presenceStatusToString(info.statusType);
        Ruqola::self()->rocketChatAccount()->createCustomUserStatus(statusCreateInfo);
    }
    delete dlg;
}

void CustomUserStatusTreeWidget::editClicked()
{
    if (!currentItem()) {
        return;
    }

    QPointer<AdministratorCustomUserStatusCreateDialog> dlg = new AdministratorCustomUserStatusCreateDialog(this);
    auto *customUserStatusItem = static_cast<CustomUserStatusTreeWidgetItem *>(currentItem());
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
        Ruqola::self()->rocketChatAccount()->updateCustomUserStatus(statusUpdateInfo);
    }
    delete dlg;
}

void CustomUserStatusTreeWidget::removeClicked()
{
    if (!currentItem()) {
        return;
    }
    auto *customUserStatusItem = static_cast<CustomUserStatusTreeWidgetItem *>(currentItem());
    const CustomUserStatus userStatus = customUserStatusItem->userStatus();
    if (KMessageBox::Yes == KMessageBox::questionYesNo(this, i18n("Do you want to remove \"%1\"?", userStatus.name()), i18n("Remove Custom User Status"))) {
        Ruqola::self()->rocketChatAccount()->removeCustomUserStatus(userStatus.identifier());
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
