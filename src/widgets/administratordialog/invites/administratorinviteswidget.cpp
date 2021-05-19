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

#include "administratorinviteswidget.h"
#include "invite/listinvitejob.h"
#include "invite/removeinvitejob.h"
#include "inviteinfo.h"
#include "invitetreeview.h"
#include "model/admininvitefilterproxymodel.h"
#include "model/admininvitemodel.h"
#include "restapirequest.h"
#include "rocketchataccount.h"
#include "ruqola.h"
#include "ruqolawidgets_debug.h"
#include <QHeaderView>
#include <QLineEdit>
#include <QVBoxLayout>

AdministratorInvitesWidget::AdministratorInvitesWidget(QWidget *parent)
    : QWidget(parent)
    , mInviteTreeView(new InviteTreeView(this))
    , mSearchLineWidget(new QLineEdit(this))
    , mAdminInviteModel(new AdminInviteModel(this))
{
    mAdminInviteFilterProxyModel = new AdminInviteFilterProxyModel(mAdminInviteModel, this);
    mAdminInviteFilterProxyModel->setObjectName(QStringLiteral("mAdminInviteFilterProxyModel"));

    auto mainLayout = new QVBoxLayout(this);
    mainLayout->setObjectName(QStringLiteral("mainLayout"));

    mSearchLineWidget->setObjectName(QStringLiteral("mSearchLineWidget"));
    mainLayout->addWidget(mSearchLineWidget);

    mInviteTreeView->setObjectName(QStringLiteral("mInviteTreeWidget"));
    mainLayout->addWidget(mInviteTreeView);
    mInviteTreeView->setModel(mAdminInviteFilterProxyModel);
    initialize();
    connect(mInviteTreeView, &InviteTreeView::removeInvite, this, &AdministratorInvitesWidget::slotRemoveInvite);
    connect(mSearchLineWidget, &QLineEdit::textChanged, this, &AdministratorInvitesWidget::slotTextChanged);

    // Hide not useful columns
    mInviteTreeView->setColumnHidden(AdminInviteModel::AdminInviteRoles::UserIdentifier, true);
    mInviteTreeView->setColumnHidden(AdminInviteModel::AdminInviteRoles::RoomId, true);
}

AdministratorInvitesWidget::~AdministratorInvitesWidget()
{
}

void AdministratorInvitesWidget::slotTextChanged(const QString &str)
{
    mAdminInviteFilterProxyModel->setFilterString(str);
}

void AdministratorInvitesWidget::initialize()
{
    auto *rcAccount = Ruqola::self()->rocketChatAccount();
    auto inviteJob = new RocketChatRestApi::ListInviteJob(this);
    rcAccount->restApi()->initializeRestApiJob(inviteJob);
    connect(inviteJob, &RocketChatRestApi::ListInviteJob::listInviteDone, this, &AdministratorInvitesWidget::slotListInviteDone);
    if (!inviteJob->start()) {
        qCDebug(RUQOLAWIDGETS_LOG) << "Impossible to start ListInviteJob";
    }
}

void AdministratorInvitesWidget::slotListInviteDone(const QJsonDocument &obj)
{
    QVector<InviteInfo> lstInvite;
    const QJsonArray array = obj.array();
    for (int i = 0; i < array.count(); ++i) {
        const QJsonObject o = array.at(i).toObject();
        InviteInfo invite;
        invite.parseInviteInfo(o);
        lstInvite.append(invite);
    }
    mAdminInviteModel->setAdminInvites(lstInvite);
    // qDebug() << " lstInvite " << lstInvite;
    // qDebug() << " obj " << obj;
}

void AdministratorInvitesWidget::slotRemoveInvite(const QString &identifier)
{
    auto *rcAccount = Ruqola::self()->rocketChatAccount();
    auto removeInviteJob = new RocketChatRestApi::RemoveInviteJob(this);
    removeInviteJob->setIdentifier(identifier);
    rcAccount->restApi()->initializeRestApiJob(removeInviteJob);
    connect(removeInviteJob, &RocketChatRestApi::RemoveInviteJob::removeInviteDone, this, &AdministratorInvitesWidget::slotRemoveInviteDone);
    if (!removeInviteJob->start()) {
        qCDebug(RUQOLAWIDGETS_LOG) << "Impossible to start RemoveInviteJob";
    }
}

void AdministratorInvitesWidget::slotRemoveInviteDone(const QString &identifier)
{
    mAdminInviteModel->removeInvite(identifier);
}
