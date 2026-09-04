/*
   SPDX-FileCopyrightText: 2021-2026 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "invitetreeview.h"

#include "model/admininvitemodel.h"
#include <KLocalizedString>
#include <KMessageBox>
#include <QIcon>
#include <QMenu>

using namespace Qt::Literals::StringLiterals;
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
    sortByColumn(AdminInviteModel::Identifier, Qt::AscendingOrder);
}

InviteTreeView::~InviteTreeView() = default;

void InviteTreeView::slotCustomContextMenuRequested(const QPoint &pos)
{
    const QModelIndex index = indexAt(pos);
    if (index.isValid()) {
        QMenu menu(this);
        menu.addAction(QIcon::fromTheme(u"list-remove"_s), i18nc("@action", "Remove"), this, [this, index]() {
            removeClicked(index.siblingAtColumn(AdminInviteModel::Identifier).data().toByteArray());
        });
        menu.exec(viewport()->mapToGlobal(pos));
    }
}

void InviteTreeView::removeClicked(const QByteArray &identifier)
{
    if (KMessageBox::PrimaryAction
        == KMessageBox::warningTwoActions(this,
                                          i18n("Are you sure that you want to delete this invite?"),
                                          i18nc("@title:window", "Remove Invite"),
                                          KStandardGuiItem::del(),
                                          KStandardGuiItem::cancel())) {
        Q_EMIT removeInvite(identifier);
    }
}

#include "moc_invitetreeview.cpp"
