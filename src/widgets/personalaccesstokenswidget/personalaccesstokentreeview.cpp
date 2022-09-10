/*
   SPDX-FileCopyrightText: 2022 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "personalaccesstokentreeview.h"

#include <KLocalizedString>
#include <KMessageBox>
#include <QMenu>

PersonalAccessTokenTreeView::PersonalAccessTokenTreeView(QWidget *parent)
    : QTreeView(parent)
{
    setAlternatingRowColors(true);
    setSelectionMode(SingleSelection);
    setContextMenuPolicy(Qt::CustomContextMenu);
    setSelectionBehavior(QAbstractItemView::SelectRows);
    setRootIsDecorated(false);
    setSortingEnabled(true);
    connect(this, &PersonalAccessTokenTreeView::customContextMenuRequested, this, &PersonalAccessTokenTreeView::slotCustomContextMenuRequested);
    connect(this, &QTreeView::doubleClicked, this, &PersonalAccessTokenTreeView::editClicked);
    // TODO initialize();
}

PersonalAccessTokenTreeView::~PersonalAccessTokenTreeView() = default;

void PersonalAccessTokenTreeView::slotCustomContextMenuRequested(const QPoint &pos)
{
#if 0
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
#endif
}

void PersonalAccessTokenTreeView::removeClicked(const QString &identifier)
{
#if 0
    if (KMessageBox::Yes
        == KMessageBox::warningYesNo(this, i18n("Are you sure that you want to delete this Oauth?"), i18n("Remove OAuth"), KStandardGuiItem::remove())) {
        Q_EMIT removeOauth(identifier);
    }
#endif
}

void PersonalAccessTokenTreeView::addClicked()
{
#if 0
    QPointer<AdministratorOauthCreateDialog> dlg = new AdministratorOauthCreateDialog(this);
    if (dlg->exec()) {
        const AdministratorOauthCreateWidget::OauthCreateInfo info = dlg->oauthInfo();
        mRocketChatAccount->ddp()->addOAuthApp(info.applicationName, info.active, info.redirectUrl);
    }
    delete dlg;
#endif
}

void PersonalAccessTokenTreeView::editClicked()
{
    // TODO
}
