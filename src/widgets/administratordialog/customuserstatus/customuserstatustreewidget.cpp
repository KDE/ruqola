/*
   Copyright (c) 2020 Laurent Montel <montel@kde.org>

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

#include "customuserstatustreewidget.h"
#include <KLocalizedString>
#include <QHeaderView>
#include <QMenu>


CustomUserStatusTreeWidget::CustomUserStatusTreeWidget(QWidget *parent)
    : QTreeWidget(parent)
{
    setColumnCount(2);
    setHeaderLabels({i18n("Name"), i18n("Presence")});
    setAlternatingRowColors(true);
    header()->setSectionsMovable(false);
    header()->setSectionResizeMode(QHeaderView::ResizeToContents);
    setSelectionMode(SingleSelection);
    connect(this, &CustomUserStatusTreeWidget::customContextMenuRequested, this, &CustomUserStatusTreeWidget::slotCustomContextMenuRequested);

}

CustomUserStatusTreeWidget::~CustomUserStatusTreeWidget()
{

}

void CustomUserStatusTreeWidget::addClicked()
{

}

void CustomUserStatusTreeWidget::editClicked()
{

}

void CustomUserStatusTreeWidget::removeClicked()
{

}

void CustomUserStatusTreeWidget::slotCustomContextMenuRequested(const QPoint &pos)
{
    QMenu menu(this);
    menu.addAction(QIcon::fromTheme(QStringLiteral("list-add")), i18n("Add..."), this, &CustomUserStatusTreeWidget::addClicked);
    QTreeWidgetItem *item = itemAt(pos);
    if (item) {
        menu.addAction(QIcon::fromTheme(QStringLiteral("document-edit")), i18n("Modify..."), this, &CustomUserStatusTreeWidget::editClicked);
        menu.addAction(QIcon::fromTheme(QStringLiteral("list-remove")), i18n("Remove"), this, &CustomUserStatusTreeWidget::removeClicked);
    }
    menu.exec(viewport()->mapToGlobal(pos));

}
