/*
   SPDX-FileCopyrightText: 2021-2022 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "invitetreeview.h"
#include "model/admininvitemodel.h"
#include <KLocalizedString>
#include <KMessageBox>
#include <QMenu>
#include <kwidgetsaddons_version.h>

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
#if KWIDGETSADDONS_VERSION >= QT_VERSION_CHECK(5, 100, 0)
    if (KMessageBox::PrimaryAction
        == KMessageBox::warningTwoActions(this,
#else
    if (KMessageBox::Yes
        == KMessageBox::warningYesNo(this,
#endif
                                          i18n("Are you sure that you want to delete this invite?"),
                                          i18n("Remove Invite"),
                                          KStandardGuiItem::del(),
                                          KStandardGuiItem::cancel())) {
        Q_EMIT removeInvite(identifier);
    }
}
