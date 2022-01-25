/*
   SPDX-FileCopyrightText: 2022 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/
#include "administratoroauthwidget.h"
#include "administratoroauthfilterproxymodel.h"
#include "connection.h"
#include "misc/listoauthappsjob.h"
#include "model/adminoauthmodel.h"
#include "oauth/oauthinfo.h"
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
    , mOauthTreeWidget(new OauthTreeView(this))
    , mAdminOauthModel(new AdminOauthModel(this))
{
    auto mainLayout = new QVBoxLayout(this);
    mainLayout->setObjectName(QStringLiteral("mainLayout"));

    mSearchLineWidget->setObjectName(QStringLiteral("mSearchLineWidget"));
    mainLayout->addWidget(mSearchLineWidget);
    mSearchLineWidget->setPlaceholderText(i18n("Search Oauth Apps..."));

    mOauthTreeWidget->setObjectName(QStringLiteral("mOauthTreeWidget"));
    mainLayout->addWidget(mOauthTreeWidget);
    mAdminInviteFilterProxyModel = new AdministratorOauthFilterProxyModel(mAdminOauthModel, this);
    mAdminInviteFilterProxyModel->setObjectName(QStringLiteral("mAdminInviteFilterProxyModel"));
    mOauthTreeWidget->setModel(mAdminInviteFilterProxyModel);
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
    QVector<OauthInfo> lstOauth;
    const QJsonArray array = obj[QStringLiteral("oauthApps")].toArray();
    const auto arrayCount{array.count()};
    lstOauth.reserve(arrayCount);
#if 0
    for (auto i = 0; i < arrayCount; ++i) {
        const QJsonObject o = array.at(i).toObject();
        OauthInfo info;
        invite.parseInviteInfo(o);
        lstOauth.append(info);
    }
    mAdminInviteModel->setAdminInvites(lstInvite);
    // qDebug() << " lstInvite " << lstInvite;
    // qDebug() << " obj " << obj;
    for (int i : {AdminInviteModel::AdminInviteRoles::CreateStr, AdminInviteModel::AdminInviteRoles::Identifier}) {
        mInviteTreeView->resizeColumnToContents(i);
    }
#endif
}
