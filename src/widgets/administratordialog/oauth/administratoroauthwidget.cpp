/*
   SPDX-FileCopyrightText: 2022 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/
#include "administratoroauthwidget.h"
#include "connection.h"
#include "misc/listoauthappsjob.h"
#include "model/adminoauthmodel.h"
#include "oauthtreeview.h"
#include "rocketchataccount.h"
#include "ruqola.h"
#include "ruqolawidgets_debug.h"
#include <KLocalizedString>
#include <QLineEdit>
#include <QVBoxLayout>

AdministratorOauthWidget::AdministratorOauthWidget(RocketChatAccount *account, QWidget *parent)
    : QWidget{parent}
    , mRocketChatAccount(account)
    , mSearchLineWidget(new QLineEdit(this))
    , mOauthTreeView(new OauthTreeView(this))
    , mAdminInviteModel(new AdminOauthModel(this))
{
    auto mainLayout = new QVBoxLayout(this);
    mainLayout->setObjectName(QStringLiteral("mainLayout"));

    mSearchLineWidget->setObjectName(QStringLiteral("mSearchLineWidget"));
    mainLayout->addWidget(mSearchLineWidget);
    mSearchLineWidget->setPlaceholderText(i18n("Search Oauth Apps..."));

    mOauthTreeView->setObjectName(QStringLiteral("mOauthTreeView"));
    mainLayout->addWidget(mOauthTreeView);
    // mAdminInviteFilterProxyModel = new AdministratorInvitesFilterProxyModel(mAdminInviteModel, this);
    // mAdminInviteFilterProxyModel->setObjectName(QStringLiteral("mAdminInviteFilterProxyModel"));
    // TODO mInviteTreeView->setModel(mAdminInviteFilterProxyModel);
    // TODO add model
}

AdministratorOauthWidget::~AdministratorOauthWidget() = default;

void AdministratorOauthWidget::initialize()
{
    auto oauthListJob = new RocketChatRestApi::ListOauthAppsJob(this);
    mRocketChatAccount->restApi()->initializeRestApiJob(oauthListJob);
    connect(oauthListJob, &RocketChatRestApi::ListOauthAppsJob::listOauthDone, this, &AdministratorOauthWidget::slotListOauthDone);
    if (!oauthListJob->start()) {
        qCDebug(RUQOLAWIDGETS_LOG) << "Impossible to start ListInviteJob";
    }
}

void AdministratorOauthWidget::slotListOauthDone(const QJsonObject &obj)
{
    qDebug() << " obj " << obj;
}
