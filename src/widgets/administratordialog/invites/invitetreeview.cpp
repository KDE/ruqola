/*
   SPDX-FileCopyrightText: 2021-2023 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "invitetreeview.h"
#include "model/admininvitemodel.h"
#include <KLocalizedString>
#include <KMessageBox>
#include <QMenu>

InviteTreeView::InviteTreeView(QWidget *parent)
    : QTreeView(parent)
{
    setAlternatingRowColors(true);
    setSelectionMode(SingleSelection);
    setContextMenuPolicy(Qt::CustomContextMenu);
    setSelectionBehavior(QAbstractItemView::SelectRows);
    setRootIsDecorated(false);
    setSortingEnabled(true);
    connect(this, &InviteTreeView::customContextMenuRequested, this, &InviteTreeView::slotCustomContextMenuRequested);
    initialize();
}

InviteTreeView::~InviteTreeView() = default;

void InviteTreeView::initialize()
{
    //    clear();
    sortByColumn(0, Qt::AscendingOrder);
}

void InviteTreeView::slotCustomContextMenuRequested(const QPoint &pos)
{
    const QModelIndex index = indexAt(pos);
    if (index.isValid()) {
        QMenu menu(this);
        menu.addAction(QIcon::fromTheme(QStringLiteral("list-remove")), i18n("Remove"), this, [this, index]() {
            const QModelIndex modelIndex = model()->index(index.row(), AdminInviteModel::Identifier);
            removeClicked(modelIndex.data().toString());
        });
        menu.exec(viewport()->mapToGlobal(pos));
    }
}

void InviteTreeView::removeClicked(const QString &identifier)
{
    if (KMessageBox::PrimaryAction
        == KMessageBox::warningTwoActions(this,
                                          i18n("Are you sure that you want to delete this invite?"),
                                          i18n("Remove Invite"),
                                          KStandardGuiItem::del(),
                                          KStandardGuiItem::cancel())) {
        Q_EMIT removeInvite(identifier);
    }
}

#include "moc_invitetreeview.cpp"
