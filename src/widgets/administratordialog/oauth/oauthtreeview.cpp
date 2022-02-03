/*
   SPDX-FileCopyrightText: 2022 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "oauthtreeview.h"
#include "administratoroauthcreatedialog.h"
#include "administratoroautheditdialog.h"
#include "model/adminoauthmodel.h"
#include "rocketchataccount.h"
#include "ruqola.h"
#include <KLocalizedString>
#include <KMessageBox>
#include <QMenu>
#include <QPointer>

OauthTreeView::OauthTreeView(QWidget *parent)
    : QTreeView(parent)
{
    setAlternatingRowColors(true);
    setSelectionMode(SingleSelection);
    setContextMenuPolicy(Qt::CustomContextMenu);
    setSelectionBehavior(QAbstractItemView::SelectRows);
    setRootIsDecorated(false);
    setSortingEnabled(true);
    connect(this, &OauthTreeView::customContextMenuRequested, this, &OauthTreeView::slotCustomContextMenuRequested);
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
        menu.addAction(QIcon::fromTheme(QStringLiteral("document-edit")), i18n("Modify..."), this, &OauthTreeView::editClicked);
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
    if (KMessageBox::Yes == KMessageBox::warningYesNo(this, i18n("Are you sure that you want to delete this Oauth?"), i18n("Remove OAuth"))) {
        Q_EMIT removeOauth(identifier);
    }
}

void OauthTreeView::addClicked()
{
    QPointer<AdministratorOauthCreateDialog> dlg = new AdministratorOauthCreateDialog(this);
    if (dlg->exec()) {
        const AdministratorOauthCreateWidget::OauthCreateInfo info = dlg->oauthInfo();
        Ruqola::self()->rocketChatAccount()->ddp()->addOAuthApp(info.applicationName, info.active, info.redirectUrl);
    }
    delete dlg;
}

void OauthTreeView::editClicked()
{
    QPointer<AdministratorOauthEditDialog> dlg = new AdministratorOauthEditDialog(this);
    AdministratorOauthEditWidget::OauthEditInfo info; // TODO
    if (dlg->exec()) {
        info = dlg->oauthInfo();
        if (info.isValid()) {
            Ruqola::self()->rocketChatAccount()->ddp()->updateOAuthApp(info.applicationName, info.active, info.redirectUrl);
        }
    }
    delete dlg;
}
