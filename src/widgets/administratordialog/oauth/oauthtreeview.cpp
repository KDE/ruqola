/*
   SPDX-FileCopyrightText: 2022 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "oauthtreeview.h"
#include "model/admininvitemodel.h"
#include <KLocalizedString>
#include <KMessageBox>
#include <QMenu>

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
            const QModelIndex modelIndex = model()->index(index.row(), AdminInviteModel::Identifier);
            removeClicked(modelIndex.data().toString());
        });
        menu.exec(viewport()->mapToGlobal(pos));
    }
}

void OauthTreeView::removeClicked(const QString &identifier)
{
    if (KMessageBox::Yes == KMessageBox::warningYesNo(this, i18n("Are you sure that you want to delete this invite?"), i18n("Remove Invite"))) {
        Q_EMIT removeInvite(identifier);
    }
}

void OauthTreeView::addClicked()
{
    // TODO
}

void OauthTreeView::editClicked()
{
    // TODO
}
