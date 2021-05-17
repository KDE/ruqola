/*
   Copyright (c) 2021 Laurent Montel <montel@kde.org>

   This library is free software; you can redistribute it and/or modify
   it under the terms of the GNU Library General Public License as published
   by the Free Software Foundation; either version 2 of the License or
   ( at your option ) version 3 or, at the discretion of KDE e.V.
   ( which shall act as a proxy as in section 14 of the GPLv3 ), any later version.

   This library is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
   Library General Public License for more details.

   You should have received a copy of the GNU Library General Public License
   along with this library; see the file COPYING.LIB.  If not, write to
   the Free Software Foundation, Inc., 51 Franklin Street, Fifth Floor,
   Boston, MA 02110-1301, USA.
*/

#include "invitetableview.h"
#include "model/admininvitemodel.h"
#include <KLocalizedString>
#include <KMessageBox>
#include <QHeaderView>
#include <QMenu>

InviteTableView::InviteTableView(QWidget *parent)
    : QTableView(parent)
{
    setAlternatingRowColors(true);
    setSelectionMode(SingleSelection);
    setContextMenuPolicy(Qt::CustomContextMenu);
    setSelectionBehavior(QAbstractItemView::SelectRows);
    setSortingEnabled(true);
    connect(this, &InviteTableView::customContextMenuRequested, this, &InviteTableView::slotCustomContextMenuRequested);
    initialize();
}

InviteTableView::~InviteTableView()
{
}

void InviteTableView::initialize()
{
    //    clear();
    sortByColumn(0, Qt::AscendingOrder);
}

void InviteTableView::slotCustomContextMenuRequested(const QPoint &pos)
{
    const QModelIndex index = indexAt(pos);
    qDebug() << index;
    if (index.isValid()) {
        QMenu menu(this);
        menu.addAction(QIcon::fromTheme(QStringLiteral("list-remove")), i18n("Remove"), this, [this, index]() {
            removeClicked(index.data(AdminInviteModel::Identifier).toString());
        });
        menu.exec(viewport()->mapToGlobal(pos));
    }
}

void InviteTableView::removeClicked(const QString &identifier)
{
    if (KMessageBox::Yes == KMessageBox::warningYesNo(this, i18n("Are you sure that you want to delete this invite?"), i18n("Remove Invite"))) {
        Q_EMIT removeInvite(identifier);
    }
}
