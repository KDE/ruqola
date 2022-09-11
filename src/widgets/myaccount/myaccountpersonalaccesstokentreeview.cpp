/*
   SPDX-FileCopyrightText: 2022 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "myaccountpersonalaccesstokentreeview.h"
#include "rocketchataccount.h"

#include <KLocalizedString>
#include <KMessageBox>
#include <QMenu>

MyAccountPersonalAccessTokenTreeView::MyAccountPersonalAccessTokenTreeView(RocketChatAccount *account, QWidget *parent)
    : QTreeView(parent)
    , mRocketAccount(account)
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
    connect(this, &QTreeView::doubleClicked, this, &MyAccountPersonalAccessTokenTreeView::regenerateTokenClicked);
}

MyAccountPersonalAccessTokenTreeView::~MyAccountPersonalAccessTokenTreeView() = default;

void MyAccountPersonalAccessTokenTreeView::slotCustomContextMenuRequested(const QPoint &pos)
{
    const QModelIndex index = indexAt(pos);
    QMenu menu(this);
    menu.addAction(QIcon::fromTheme(QStringLiteral("list-add")), i18n("Add..."), this, &MyAccountPersonalAccessTokenTreeView::addClicked);
    if (index.isValid()) {
        menu.addAction(QIcon::fromTheme(QStringLiteral("document-edit")), i18n("Regenerate..."), this, [this, index]() {
            regenerateTokenClicked(index);
        });
        menu.addSeparator();
        menu.addAction(QIcon::fromTheme(QStringLiteral("list-remove")), i18n("Remove"), this, [this, index]() {
            // const QModelIndex modelIndex = model()->index(index.row(), AdminOauthModel::Identifier);
            // removeClicked(modelIndex.data().toString());
        });
    }
    menu.exec(viewport()->mapToGlobal(pos));
}

void MyAccountPersonalAccessTokenTreeView::removeClicked(const QString &identifier)
{
    if (KMessageBox::Yes
        == KMessageBox::warningYesNo(this, i18n("Are you sure that you want to delete this Token?"), i18n("Remove Token"), KStandardGuiItem::remove())) {
        Q_EMIT removeToken(identifier);
    }
}

void MyAccountPersonalAccessTokenTreeView::addClicked()
{
    // TODO
}

void MyAccountPersonalAccessTokenTreeView::regenerateTokenClicked(const QModelIndex &index)
{
    // TODO
}
