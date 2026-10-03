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
using namespace Qt::Literals::StringLiterals;
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
    setExpandsOnDoubleClick(false);
    setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);

    connect(model(), &QAbstractItemModel::rowsInserted, this, &QTreeView::expandAll);
    connect(model(), &QAbstractItemModel::modelReset, this, &QTreeView::expandAll);
    connect(model(), &QAbstractItemModel::rowsMoved, this, &QTreeView::expandAll);
    connect(model(), &QAbstractItemModel::layoutChanged, this, &QTreeView::expandAll);

    connect(model(), &QAbstractItemModel::rowsInserted, this, &UsersInRoomTreeView::updateSectionVisibility);
    connect(model(), &QAbstractItemModel::rowsRemoved, this, &UsersInRoomTreeView::updateSectionVisibility);
    connect(model(), &QAbstractItemModel::rowsMoved, this, &UsersInRoomTreeView::updateSectionVisibility);
    connect(model(), &QAbstractItemModel::layoutChanged, this, &UsersInRoomTreeView::updateSectionVisibility);
    connect(model(), &QAbstractItemModel::modelReset, this, &UsersInRoomTreeView::updateSectionVisibility);
    connect(model(), &QAbstractItemModel::dataChanged, this, &UsersInRoomTreeView::updateSectionVisibility);

    expandAll();
    updateSectionVisibility();
}

UsersInRoomTreeView::~UsersInRoomTreeView() = default;

void UsersInRoomTreeView::paintEvent(QPaintEvent *prevent)
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
        QTreeView::paintEvent(prevent);
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

void UsersInRoomTreeView::updateSectionVisibility()
{
    if (!model()) {
        return;
    }

    const int sectionCount = model()->rowCount();
    for (int row = 0; row < sectionCount; ++row) {
        const QModelIndex sectionIndex = model()->index(row, 0);
        const bool hasChildren = model()->rowCount(sectionIndex) > 0;
        setRowHidden(row, QModelIndex(), !hasChildren);
    }
}

bool UsersInRoomTreeView::event(QEvent *ev)
{
    if (ev->type() == QEvent::ApplicationPaletteChange) {
        generalPaletteChanged();
    }
    return QTreeView::event(ev);
}

#include "moc_usersinroomtreeview.cpp"
