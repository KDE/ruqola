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

#include "invitetreewidget.h"
#include <KLocalizedString>
#include <QHeaderView>
#include <QMenu>

InviteTreeWidget::InviteTreeWidget(QWidget *parent)
    : QTreeWidget(parent)
{
    setColumnCount(5);
    setHeaderLabels({i18n("Token"), i18n("Created at"), i18n("Expiration"), i18n("Uses"), i18n("Uses left")});
    setAlternatingRowColors(true);
    header()->setSectionsMovable(false);
    header()->setSectionResizeMode(QHeaderView::ResizeToContents);
    setSelectionMode(SingleSelection);
    setContextMenuPolicy(Qt::CustomContextMenu);
    setRootIsDecorated(false);
    connect(this, &InviteTreeWidget::customContextMenuRequested, this, &InviteTreeWidget::slotCustomContextMenuRequested);
    initialize();
}

InviteTreeWidget::~InviteTreeWidget()
{
}

void InviteTreeWidget::initialize()
{
    clear();
    // TODO
    header()->setSortIndicatorShown(true);
    header()->setSectionsClickable(true);
    sortByColumn(0, Qt::AscendingOrder);
}

void InviteTreeWidget::slotCustomContextMenuRequested(const QPoint &pos)
{
    QTreeWidgetItem *item = itemAt(pos);
    if (item) {
        QMenu menu(this);
        menu.addAction(QIcon::fromTheme(QStringLiteral("list-remove")), i18n("Remove"), this, &InviteTreeWidget::removeClicked);
        menu.exec(viewport()->mapToGlobal(pos));
    }
}

void InviteTreeWidget::removeClicked()
{
    if (!currentItem()) {
        return;
    }
    // TODO
}
