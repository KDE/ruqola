/*
   SPDX-FileCopyrightText: 2022-2025 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/
#include "administratoroauthwidget.h"

#include "connection.h"
#include "ddpapi/ddpclient.h"
#include "misc/listoauthappsjob.h"
#include "model/administratoroauthfilterproxymodel.h"
#include "model/adminoauthmodel.h"
#include "oauth/oauthinfo.h"
#include "oauthtreeview.h"
#include "rocketchataccount.h"
#include "ruqolawidgets_debug.h"
#include <KLineEditEventHandler>
#include <KLocalizedString>
#include <QLineEdit>
#include <QVBoxLayout>

using namespace Qt::Literals::StringLiterals;
AdministratorOauthWidget::AdministratorOauthWidget(RocketChatAccount *account, QWidget *parent)
    : QWidget{parent}
    , mRocketChatAccount(account)
    , mSearchLineWidget(new QLineEdit(this))
    , mOauthTreeWidget(new OauthTreeView(mRocketChatAccount, this))
    , mAdminOauthModel(new AdminOauthModel(this))
{
    auto mainLayout = new QVBoxLayout(this);
    mainLayout->setObjectName(u"mainLayout"_s);
    mainLayout->setContentsMargins({});
    mainLayout->setSpacing(0);

    mSearchLineWidget->setObjectName(u"mSearchLineWidget"_s);
    mainLayout->addWidget(mSearchLineWidget);
    mSearchLineWidget->setPlaceholderText(i18nc("@info:placeholder", "Search Oauth apps…"));
    KLineEditEventHandler::catchReturnKey(mSearchLineWidget);
    mOauthTreeWidget->setObjectName(u"mOauthTreeWidget"_s);
    mainLayout->addWidget(mOauthTreeWidget);
    mAdminInviteFilterProxyModel = new AdministratorOauthFilterProxyModel(mAdminOauthModel, this);
    mAdminInviteFilterProxyModel->setObjectName(u"mAdminInviteFilterProxyModel"_s);
    mOauthTreeWidget->setModel(mAdminInviteFilterProxyModel);
    connect(mSearchLineWidget, &QLineEdit::textChanged, this, &AdministratorOauthWidget::slotTextChanged);
    connect(mOauthTreeWidget, &OauthTreeView::removeOauth, this, &AdministratorOauthWidget::slotRemoveOauth);
    connect(mOauthTreeWidget, &OauthTreeView::oauthAdded, this, &AdministratorOauthWidget::slotOauthAppAdded);
    connect(mOauthTreeWidget, &OauthTreeView::oauthUpdated, this, &AdministratorOauthWidget::slotOauthAppUpdated);

    // Hide not useful columns
    mOauthTreeWidget->setColumnHidden(AdminOauthModel::AdminOauthRoles::ClientId, true);
    mOauthTreeWidget->setColumnHidden(AdminOauthModel::AdminOauthRoles::ClientSecret, true);
    mOauthTreeWidget->setColumnHidden(AdminOauthModel::AdminOauthRoles::RedirectUri, true);
    mOauthTreeWidget->setColumnHidden(AdminOauthModel::AdminOauthRoles::CreatedAt, true);
    mOauthTreeWidget->setColumnHidden(AdminOauthModel::AdminOauthRoles::Identifier, true);
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

void AdministratorOauthWidget::slotOauthAppAdded(const QJsonObject &obj)
{
    OauthInfo info;
    info.parseOauthInfo(std::move(obj), true);
    mAdminOauthModel->addMoreOauth(info);
}

void AdministratorOauthWidget::slotOauthAppUpdated(const QJsonObject &obj)
{
    OauthInfo info;
    info.parseOauthInfo(std::move(obj), true);
    mAdminOauthModel->removeOauth(info.identifier());
    mAdminOauthModel->addMoreOauth(info);
}

void AdministratorOauthWidget::slotListOauthDone(const QJsonObject &obj)
{
    QList<OauthInfo> lstOauth;
    const QJsonArray array = obj["oauthApps"_L1].toArray();
    const auto arrayCount{array.count()};
    lstOauth.reserve(arrayCount);
    for (auto i = 0; i < arrayCount; ++i) {
        const QJsonObject o = array.at(i).toObject();
        OauthInfo info;
        info.parseOauthInfo(o);
        lstOauth.append(std::move(info));
    }
    mAdminOauthModel->setAdminOauth(lstOauth);
    // qDebug() << " lstOauth " << lstOauth;
    // qDebug() << " obj " << obj;
    for (int i : {AdminOauthModel::AdminOauthRoles::Identifier, AdminOauthModel::AdminOauthRoles::CreatedAtStr}) {
        mOauthTreeWidget->resizeColumnToContents(i);
    }
}

void AdministratorOauthWidget::slotTextChanged(const QString &str)
{
    mAdminInviteFilterProxyModel->setFilterString(str);
}

void AdministratorOauthWidget::slotRemoveOauth(const QString &identifier)
{
    mAdminOauthModel->removeOauth(identifier);
}

#include "moc_administratoroauthwidget.cpp"
