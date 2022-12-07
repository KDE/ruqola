/*
   SPDX-FileCopyrightText: 2022 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "oauthtreeview.h"
#include "administratoroauthcreatedialog.h"
#include "administratoroautheditdialog.h"
#include "connection.h"
#include "misc/oauthappscreatejob.h"
#include "model/adminoauthmodel.h"
#include "rocketchataccount.h"
#include "ruqolawidgets_debug.h"
#include <KLocalizedString>
#include <KMessageBox>
#include <QMenu>
#include <QPointer>
#include <kwidgetsaddons_version.h>

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
    menu.addAction(QIcon::fromTheme(QStringLiteral("list-add")), i18n("Add..."), this, &OauthTreeView::addClicked);
    if (index.isValid()) {
        menu.addAction(QIcon::fromTheme(QStringLiteral("document-edit")), i18n("Modify..."), this, [this, index]() {
            editClicked(index);
        });
        menu.addSeparator();
        menu.addAction(QIcon::fromTheme(QStringLiteral("list-remove")), i18n("Remove"), this, [this, index]() {
            const QModelIndex modelIndex = model()->index(index.row(), AdminOauthModel::Identifier);
            removeClicked(modelIndex.data().toString());
        });
    }
    menu.exec(viewport()->mapToGlobal(pos));
}

void OauthTreeView::removeClicked(const QString &identifier)
{
#if KWIDGETSADDONS_VERSION >= QT_VERSION_CHECK(5, 100, 0)
    if (KMessageBox::ButtonCode::PrimaryAction
        == KMessageBox::questionTwoActions(this,
#else
    if (KMessageBox::Yes
        == KMessageBox::warningYesNo(this,
#endif
                                           i18n("Are you sure that you want to delete this Oauth?"),
                                           i18n("Remove OAuth"),
                                           KStandardGuiItem::remove(),
                                           KStandardGuiItem::cancel())) {
        Q_EMIT removeOauth(identifier);
    }
}

void OauthTreeView::addClicked()
{
    QPointer<AdministratorOauthCreateDialog> dlg = new AdministratorOauthCreateDialog(this);
    if (dlg->exec()) {
        const AdministratorOauthCreateWidget::OauthCreateInfo info = dlg->oauthInfo();
        if (mRocketChatAccount->ruqolaServerConfig()->hasAtLeastVersion(5, 4, 0)) {
            RocketChatRestApi::OauthAppsCreateJob::OauthAppsCreateInfo oauthInfo;
            oauthInfo.active = info.active;
            oauthInfo.redirectUri = info.redirectUrl;
            oauthInfo.name = info.applicationName;
            auto job = new RocketChatRestApi::OauthAppsCreateJob(this);
            job->setOauthAppsCreateInfo(oauthInfo);
            mRocketChatAccount->restApi()->initializeRestApiJob(job);
            connect(job, &RocketChatRestApi::OauthAppsCreateJob::oauthAppsCreateDone, this, &OauthTreeView::oauthAdded);
            if (!job->start()) {
                qCWarning(RUQOLAWIDGETS_LOG) << "Impossible to start OauthAppsCreateJob job";
            }
        } else {
            mRocketChatAccount->ddp()->addOAuthApp(info.applicationName, info.active, info.redirectUrl);
        }
    }
    delete dlg;
}

void OauthTreeView::editClicked(const QModelIndex &index)
{
    if (index.isValid()) {
        QPointer<AdministratorOauthEditDialog> dlg = new AdministratorOauthEditDialog(this);
        AdministratorOauthEditWidget::OauthEditInfo info;
        info.applicationName = model()->index(index.row(), AdminOauthModel::Identifier).data().toString();
        info.redirectUrl = model()->index(index.row(), AdminOauthModel::RedirectUri).data().toString();
        info.clientId = model()->index(index.row(), AdminOauthModel::ClientId).data().toString();
        info.clientSecret = model()->index(index.row(), AdminOauthModel::ClientSecret).data().toString();
        info.authorizationUrl = mRocketChatAccount->serverUrl() + QStringLiteral("/oauth/authorize");
        info.accessTokenUrl = mRocketChatAccount->serverUrl() + QStringLiteral("/oauth/token");
        info.active = model()->index(index.row(), AdminOauthModel::Active).data().toBool();
        dlg->setOauthInfo(info);
        if (dlg->exec()) {
            info = dlg->oauthInfo();
            if (info.isValid()) {
                mRocketChatAccount->ddp()->updateOAuthApp(info.applicationName, info.active, info.redirectUrl);
            }
        }
        delete dlg;
    }
}
