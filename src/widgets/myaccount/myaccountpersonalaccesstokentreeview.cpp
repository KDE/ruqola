/*
   SPDX-FileCopyrightText: 2022 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "myaccountpersonalaccesstokentreeview.h"
#include "model/personalaccesstokeninfosmodel.h"
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
}

MyAccountPersonalAccessTokenTreeView::~MyAccountPersonalAccessTokenTreeView() = default;

void MyAccountPersonalAccessTokenTreeView::slotCustomContextMenuRequested(const QPoint &pos)
{
    const QModelIndex index = indexAt(pos);
    QMenu menu(this);
    menu.addAction(QIcon::fromTheme(QStringLiteral("list-add")), i18n("Add..."), this, &MyAccountPersonalAccessTokenTreeView::createToken);
    if (index.isValid()) {
        menu.addAction(QIcon::fromTheme(QStringLiteral("view-refresh")), i18n("Regenerate..."), this, [this, index]() {
            const QModelIndex modelIndex = model()->index(index.row(), PersonalAccessTokenInfosModel::Name);
            Q_EMIT regenerateToken(modelIndex.data().toString());
        });
        menu.addSeparator();
        menu.addAction(QIcon::fromTheme(QStringLiteral("list-remove")), i18n("Remove"), this, [this, index]() {
            const QModelIndex modelIndex = model()->index(index.row(), PersonalAccessTokenInfosModel::Name);
            removeClicked(modelIndex.data().toString());
        });
    }
    menu.exec(viewport()->mapToGlobal(pos));
}

void MyAccountPersonalAccessTokenTreeView::removeClicked(const QString &tokenName)
{
    if (KMessageBox::Yes
        == KMessageBox::warningYesNo(this,
                                     i18n("Are you sure that you want to delete \"%1\" Token?", tokenName),
                                     i18n("Remove Token"),
                                     KStandardGuiItem::remove())) {
        Q_EMIT removeToken(tokenName);
    }
}
