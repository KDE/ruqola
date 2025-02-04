/*
   SPDX-FileCopyrightText: 2022-2025 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "myaccountpersonalaccesstokentreeview.h"
#include "model/personalaccesstokeninfosmodel.h"

#include <KLocalizedString>
#include <KMessageBox>
#include <QMenu>

MyAccountPersonalAccessTokenTreeView::MyAccountPersonalAccessTokenTreeView(QWidget *parent)
    : QTreeView(parent)
{
    setAlternatingRowColors(true);
    setSelectionMode(SingleSelection);
    setContextMenuPolicy(Qt::CustomContextMenu);
    setSelectionBehavior(QAbstractItemView::SelectRows);
    setRootIsDecorated(false);
    setSortingEnabled(true);
    connect(this,
            &MyAccountPersonalAccessTokenTreeView::customContextMenuRequested,
            this,
            &MyAccountPersonalAccessTokenTreeView::slotCustomContextMenuRequested);
}

MyAccountPersonalAccessTokenTreeView::~MyAccountPersonalAccessTokenTreeView() = default;

void MyAccountPersonalAccessTokenTreeView::slotCustomContextMenuRequested(const QPoint &pos)
{
    const QModelIndex index = indexAt(pos);
    QMenu menu(this);
    menu.addAction(QIcon::fromTheme(QStringLiteral("list-add")), i18nc("@action", "Add…"), this, &MyAccountPersonalAccessTokenTreeView::createToken);
    if (index.isValid()) {
        menu.addAction(QIcon::fromTheme(QStringLiteral("view-refresh")), i18nc("@action", "Regenerate…"), this, [this, index]() {
            const QModelIndex modelIndex = model()->index(index.row(), PersonalAccessTokenInfosModel::Name);
            Q_EMIT regenerateToken(modelIndex.data().toString());
        });
        menu.addSeparator();
        menu.addAction(QIcon::fromTheme(QStringLiteral("list-remove")), i18nc("@action", "Remove"), this, [this, index]() {
            const QModelIndex modelIndex = model()->index(index.row(), PersonalAccessTokenInfosModel::Name);
            removeClicked(modelIndex.data().toString());
        });
    }
    menu.exec(viewport()->mapToGlobal(pos));
}

void MyAccountPersonalAccessTokenTreeView::removeClicked(const QString &tokenName)
{
    if (KMessageBox::ButtonCode::PrimaryAction
        == KMessageBox::questionTwoActions(this,
                                           i18n("Are you sure that you want to delete \"%1\" Token?", tokenName),
                                           i18nc("@title:window", "Remove Token"),
                                           KStandardGuiItem::remove(),
                                           KStandardGuiItem::cancel())) {
        Q_EMIT removeToken(tokenName);
    }
}

#include "moc_myaccountpersonalaccesstokentreeview.cpp"
