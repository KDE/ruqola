/*
   SPDX-FileCopyrightText: 2022-2026 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "oauthtreeview.h"
using namespace Qt::Literals::StringLiterals;

#include "administratoroauthcreatedialog.h"
#include "administratoroautheditdialog.h"
#include "connection.h"
#include "misc/oauthappscreatejob.h"
#include "misc/oauthappsdeletejob.h"
#include "misc/oauthappsupdatejob.h"
#include "model/adminoauthmodel.h"
#include "rocketchataccount.h"
#include "ruqolawidgets_debug.h"
#include <KLocalizedString>
#include <KMessageBox>
#include <QMenu>
#include <QPointer>

OauthTreeView::OauthTreeView(RocketChatAccount *account, QWidget *parent)
    : QTreeView(parent)
    , mRocketChatAccount(account)
{
    setAlternatingRowColors(true);
    setSelectionMode(SingleSelection);
    setContextMenuPolicy(Qt::CustomContextMenu);
    setSelectionBehavior(QAbstractItemView::SelectRows);
    setRootIsDecorated(false);
    setSortingEnabled(true);
    connect(this, &OauthTreeView::customContextMenuRequested, this, &OauthTreeView::slotCustomContextMenuRequested);
    connect(this, &QTreeView::doubleClicked, this, &OauthTreeView::editClicked);
    initialize();
}

OauthTreeView::~OauthTreeView() = default;

void OauthTreeView::initialize()
{
    //    clear();
    sortByColumn(0, Qt::AscendingOrder);
}

void OauthTreeView::slotCustomContextMenuRequested(const QPoint &pos)
{
    const QModelIndex index = indexAt(pos);
    QMenu menu(this);
    menu.addAction(QIcon::fromTheme(u"list-add"_s), i18nc("@action", "Add…"), this, &OauthTreeView::addClicked);
    if (index.isValid()) {
        menu.addAction(QIcon::fromTheme(u"document-edit"_s), i18nc("@action", "Modify…"), this, [this, index]() {
            editClicked(index);
        });
        menu.addSeparator();
        menu.addAction(QIcon::fromTheme(u"list-remove"_s), i18nc("@action", "Remove"), this, [this, index]() {
            const QModelIndex modelIndex = model()->index(index.row(), AdminOauthModel::Identifier);
            removeClicked(modelIndex.data().toString());
        });
    }
    menu.exec(viewport()->mapToGlobal(pos));
}

void OauthTreeView::removeClicked(const QString &identifier)
{
    if (KMessageBox::ButtonCode::PrimaryAction
        == KMessageBox::questionTwoActions(this,
                                           i18n("Are you sure that you want to delete this Oauth?"),
                                           i18nc("@title:window", "Remove OAuth"),
                                           KStandardGuiItem::remove(),
                                           KStandardGuiItem::cancel())) {
        auto job = new RocketChatRestApi::OauthAppsDeleteJob(this);
        job->setIdentifier(identifier);
        mRocketChatAccount->restApi()->initializeRestApiJob(job);
        connect(job, &RocketChatRestApi::OauthAppsDeleteJob::oauthAppsDeleteDone, this, [this, identifier]() {
            Q_EMIT removeOauth(identifier);
        });
        if (!job->start()) {
            qCWarning(RUQOLAWIDGETS_LOG) << "Impossible to start OauthAppsDeleteJob job";
        }
    }
}

void OauthTreeView::addClicked()
{
    QPointer<AdministratorOauthCreateDialog> dlg = new AdministratorOauthCreateDialog(this);
    if (dlg->exec()) {
        const AdministratorOauthCreateWidget::OauthCreateInfo info = dlg->oauthInfo();
        RocketChatRestApi::OauthAppsCreateJob::OauthAppsCreateInfo oauthInfo;
        oauthInfo.active = info.active;
        oauthInfo.redirectUri = info.redirectUrl;
        oauthInfo.name = info.applicationName;
        auto job = new RocketChatRestApi::OauthAppsCreateJob(this);
        job->setOauthAppsCreateInfo(std::move(oauthInfo));
        mRocketChatAccount->restApi()->initializeRestApiJob(job);
        connect(job, &RocketChatRestApi::OauthAppsCreateJob::oauthAppsCreateDone, this, &OauthTreeView::oauthAdded);
        if (!job->start()) {
            qCWarning(RUQOLAWIDGETS_LOG) << "Impossible to start OauthAppsCreateJob job";
        }
    }
    delete dlg;
}

void OauthTreeView::editClicked(const QModelIndex &index)
{
    if (index.isValid()) {
        QPointer<AdministratorOauthEditDialog> dlg = new AdministratorOauthEditDialog(this);
        AdministratorOauthEditWidget::OauthEditInfo info;
        info.applicationName = model()->index(index.row(), AdminOauthModel::Name).data().toString();
        const QString applicationId = model()->index(index.row(), AdminOauthModel::Identifier).data().toString();
        info.redirectUrl = model()->index(index.row(), AdminOauthModel::RedirectUri).data().toString();
        info.clientId = model()->index(index.row(), AdminOauthModel::ClientId).data().toString();
        info.clientSecret = model()->index(index.row(), AdminOauthModel::ClientSecret).data().toString();
        info.authorizationUrl = mRocketChatAccount->serverUrl() + u"/oauth/authorize"_s;
        info.accessTokenUrl = mRocketChatAccount->serverUrl() + u"/oauth/token"_s;
        info.active = model()->index(index.row(), AdminOauthModel::Active).data().toBool();
        dlg->setOauthInfo(std::move(info));
        if (dlg->exec()) {
            info = dlg->oauthInfo();
            if (info.isValid()) {
                RocketChatRestApi::OauthAppsUpdateJob::OauthAppsUpdateInfo oauthInfo;
                oauthInfo.active = info.active;
                oauthInfo.redirectUri = info.redirectUrl;
                oauthInfo.name = info.applicationName;
                oauthInfo.appId = applicationId;
                auto job = new RocketChatRestApi::OauthAppsUpdateJob(this);
                job->setOauthAppsUpdateInfo(std::move(oauthInfo));
                mRocketChatAccount->restApi()->initializeRestApiJob(job);
                connect(job, &RocketChatRestApi::OauthAppsUpdateJob::oauthAppsUpdateDone, this, &OauthTreeView::oauthUpdated);
                if (!job->start()) {
                    qCWarning(RUQOLAWIDGETS_LOG) << "Impossible to start OauthAppsUpdateJob job";
                }
            }
        }
        delete dlg;
    }
}

#include "moc_oauthtreeview.cpp"
