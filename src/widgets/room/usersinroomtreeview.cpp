/*
   SPDX-FileCopyrightText: 2025-2026 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "usersinroomtreeview.h"
#include "model/usersforroomfilterproxymodel.h"
#include "model/usersforroomlistheadingsproxymodel.h"
#include <KLocalizedString>
#include <QEvent>
#include <QPainter>
UsersInRoomTreeView::UsersInRoomTreeView(QWidget *parent)
    : QTreeView(parent)
    , mUsersForRoomListHeadingsProxyModel(new UsersForRoomListHeadingsProxyModel(this))
    , mUsersForRoomFilterProxy(new UsersForRoomFilterProxyModel(this))
{
    mUsersForRoomFilterProxy->setObjectName(u"mUsersForRoomFilterProxy"_s);
    mUsersForRoomFilterProxy->setSourceModel(mUsersForRoomListHeadingsProxyModel);
    setModel(mUsersForRoomFilterProxy);
    setHeaderHidden(true);
    setDragEnabled(false);
    setIndentation(0);
    setRootIsDecorated(false);
    setItemsExpandable(true);
    setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);

    connect(model(), &QAbstractItemModel::rowsInserted, this, &QTreeView::expandAll);
    connect(model(), &QAbstractItemModel::modelReset, this, &QTreeView::expandAll);
    connect(model(), &QAbstractItemModel::rowsMoved, this, &QTreeView::expandAll);
    connect(model(), &QAbstractItemModel::layoutChanged, this, &QTreeView::expandAll);

    expandAll();
}

UsersInRoomTreeView::~UsersInRoomTreeView() = default;

void UsersInRoomTreeView::paintEvent(QPaintEvent *event)
{
    if (model()->rowCount() == 0) {
        const QString label = i18n("No Users Found.");

        QPainter p(viewport());

        QFont font = p.font();
        font.setItalic(true);
        p.setFont(font);

        if (!mTextColor.isValid()) {
            generalPaletteChanged();
        }
        p.setPen(mTextColor);
        p.drawText(QRect(0, 0, width(), height()), Qt::AlignCenter, label);
    } else {
        QTreeView::paintEvent(event);
    }
}

void UsersInRoomTreeView::generalPaletteChanged()
{
    const QPalette palette = viewport()->palette();
    QColor color = palette.text().color();
    color.setAlpha(128);
    mTextColor = color;
}

UsersForRoomFilterProxyModel *UsersInRoomTreeView::usersForRoomFilterProxy() const
{
    return mUsersForRoomFilterProxy;
}

bool UsersInRoomTreeView::event(QEvent *ev)
{
    if (ev->type() == QEvent::ApplicationPaletteChange) {
        generalPaletteChanged();
    }
    return QTreeView::event(ev);
}

#include "moc_usersinroomtreeview.cpp"
