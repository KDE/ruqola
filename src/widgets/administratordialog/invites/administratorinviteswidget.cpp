/*
   SPDX-FileCopyrightText: 2020-2025 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "administratorinviteswidget.h"
using namespace Qt::Literals::StringLiterals;

#include "administratorinvitesfilterproxymodel.h"
#include "connection.h"
#include "invite/inviteinfo.h"
#include "invite/listinvitejob.h"
#include "invite/removeinvitejob.h"
#include "invitetreeview.h"
#include "model/admininvitemodel.h"
#include "model/searchtreebasefilterproxymodel.h"
#include "rocketchataccount.h"
#include "ruqolawidgets_debug.h"
#include <KLocalizedString>
#include <QLineEdit>
#include <QVBoxLayout>

AdministratorInvitesWidget::AdministratorInvitesWidget(RocketChatAccount *account, QWidget *parent)
    : QWidget(parent)
    , mInviteTreeView(new InviteTreeView(this))
    , mSearchLineWidget(new QLineEdit(this))
    , mAdminInviteModel(new AdminInviteModel(this))
    , mRocketChatAccount(account)
{
    mAdminInviteFilterProxyModel = new AdministratorInvitesFilterProxyModel(mAdminInviteModel, this);
    mAdminInviteFilterProxyModel->setObjectName(u"mAdminInviteFilterProxyModel"_s);
    mAdminInviteFilterProxyModel->setFilterKeyColumn(AdminInviteModel::AdminInviteRoles::Identifier);
    auto mainLayout = new QVBoxLayout(this);
    mainLayout->setObjectName(u"mainLayout"_s);
    mainLayout->setContentsMargins({});
    mainLayout->setSpacing(0);

    mSearchLineWidget->setObjectName(u"mSearchLineWidget"_s);
    mainLayout->addWidget(mSearchLineWidget);
    mSearchLineWidget->setPlaceholderText(i18nc("@info:placeholder", "Search invite…"));

    mInviteTreeView->setObjectName(u"mInviteTreeWidget"_s);
    mainLayout->addWidget(mInviteTreeView);
    mInviteTreeView->setModel(mAdminInviteFilterProxyModel);
    connect(mInviteTreeView, &InviteTreeView::removeInvite, this, &AdministratorInvitesWidget::slotRemoveInvite);
    connect(mSearchLineWidget, &QLineEdit::textChanged, this, &AdministratorInvitesWidget::slotTextChanged);

    // Hide not useful columns
    mInviteTreeView->setColumnHidden(AdminInviteModel::AdminInviteRoles::UserIdentifier, true);
    mInviteTreeView->setColumnHidden(AdminInviteModel::AdminInviteRoles::RoomId, true);
    mInviteTreeView->setColumnHidden(AdminInviteModel::AdminInviteRoles::CreateAt, true);
}

AdministratorInvitesWidget::~AdministratorInvitesWidget() = default;

void AdministratorInvitesWidget::slotTextChanged(const QString &str)
{
    mAdminInviteFilterProxyModel->setFilterString(str);
}

void AdministratorInvitesWidget::initialize()
{
    auto inviteJob = new RocketChatRestApi::ListInviteJob(this);
    mRocketChatAccount->restApi()->initializeRestApiJob(inviteJob);
    connect(inviteJob, &RocketChatRestApi::ListInviteJob::listInviteDone, this, &AdministratorInvitesWidget::slotListInviteDone);
    if (!inviteJob->start()) {
        qCDebug(RUQOLAWIDGETS_LOG) << "Impossible to start ListInviteJob";
    }
}

void AdministratorInvitesWidget::slotListInviteDone(const QJsonDocument &obj)
{
    QList<InviteInfo> lstInvite;
    const QJsonArray array = obj.array();
    const auto arrayCount{array.count()};
    lstInvite.reserve(arrayCount);
    for (auto i = 0; i < arrayCount; ++i) {
        const QJsonObject o = array.at(i).toObject();
        InviteInfo invite;
        invite.parseInviteInfo(o);
        lstInvite.append(std::move(invite));
    }
    mAdminInviteModel->setAdminInvites(lstInvite);
    // qDebug() << " lstInvite " << lstInvite;
    // qDebug() << " obj " << obj;
    for (int i : {AdminInviteModel::AdminInviteRoles::CreateAtStr, AdminInviteModel::AdminInviteRoles::Identifier}) {
        mInviteTreeView->resizeColumnToContents(i);
    }
}

void AdministratorInvitesWidget::slotRemoveInvite(const QByteArray &identifier)
{
    auto removeInviteJob = new RocketChatRestApi::RemoveInviteJob(this);
    removeInviteJob->setIdentifier(identifier);
    mRocketChatAccount->restApi()->initializeRestApiJob(removeInviteJob);
    connect(removeInviteJob, &RocketChatRestApi::RemoveInviteJob::removeInviteDone, this, &AdministratorInvitesWidget::slotRemoveInviteDone);
    if (!removeInviteJob->start()) {
        qCDebug(RUQOLAWIDGETS_LOG) << "Impossible to start RemoveInviteJob";
    }
}

void AdministratorInvitesWidget::slotRemoveInviteDone(const QByteArray &identifier)
{
    mAdminInviteModel->removeInvite(identifier);
}

#include "moc_administratorinviteswidget.cpp"
