/*
   Copyright (c) 2020-2021 Laurent Montel <montel@kde.org>

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

#include "customsoundstreewidget.h"
#include "administratorcustomsoundscreatedialog.h"
#include <KLocalizedString>
#include <KMessageBox>
#include <QHeaderView>
#include <QMenu>
#include <QPointer>
CustomSoundsTreeWidget::CustomSoundsTreeWidget(QWidget *parent)
    : QTreeWidget(parent)
{
    setColumnCount(2);
    setHeaderLabels({i18n("Name"), i18n("Sound File")});
    setAlternatingRowColors(true);
    header()->setSectionsMovable(false);
    header()->setSectionResizeMode(QHeaderView::ResizeToContents);
    setSelectionMode(SingleSelection);
    setContextMenuPolicy(Qt::CustomContextMenu);
    setRootIsDecorated(false);
    connect(this, &CustomSoundsTreeWidget::customContextMenuRequested, this, &CustomSoundsTreeWidget::slotCustomContextMenuRequested);
    connect(this, &CustomSoundsTreeWidget::itemDoubleClicked, this, &CustomSoundsTreeWidget::editClicked);
    initialize();
}

CustomSoundsTreeWidget::~CustomSoundsTreeWidget()
{
}

void CustomSoundsTreeWidget::initialize()
{
    clear();
    // TODO
    header()->setSortIndicatorShown(true);
    header()->setSectionsClickable(true);
    sortByColumn(0, Qt::AscendingOrder);
}

void CustomSoundsTreeWidget::slotCustomContextMenuRequested(const QPoint &pos)
{
    QMenu menu(this);
    menu.addAction(QIcon::fromTheme(QStringLiteral("list-add")), i18n("Add..."), this, &CustomSoundsTreeWidget::addClicked);
    QTreeWidgetItem *item = itemAt(pos);
    if (item) {
        menu.addAction(QIcon::fromTheme(QStringLiteral("document-edit")), i18n("Modify..."), this, &CustomSoundsTreeWidget::editClicked);
        menu.addSeparator();
        menu.addAction(QIcon::fromTheme(QStringLiteral("list-remove")), i18n("Remove"), this, &CustomSoundsTreeWidget::removeClicked);
    }
    menu.exec(viewport()->mapToGlobal(pos));
}

void CustomSoundsTreeWidget::addClicked()
{
    QPointer<AdministratorCustomSoundsCreateDialog> dlg = new AdministratorCustomSoundsCreateDialog(this);
    if (dlg->exec()) {
        // TODO
    }
    delete dlg;
}

void CustomSoundsTreeWidget::editClicked()
{
    if (!currentItem()) {
        return;
    }
    // TODO edit
    QPointer<AdministratorCustomSoundsCreateDialog> dlg = new AdministratorCustomSoundsCreateDialog(this);
    if (dlg->exec()) {
        // TODO
    }
    delete dlg;
}

void CustomSoundsTreeWidget::removeClicked()
{
    if (!currentItem()) {
        return;
    }
    if (KMessageBox::questionYesNo(this, i18n("Do you want to remove this sound file?"), i18n("Remove Sound")) == KMessageBox::Yes) {
        // TODO
    }
}
