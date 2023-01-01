/*
   SPDX-FileCopyrightText: 2022-2023 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/
#include "switchchanneltreeview.h"

#include <QKeyEvent>

SwitchChannelTreeView::SwitchChannelTreeView(QWidget *parent)
    : QTreeView(parent)
{
    setWindowFlags(Qt::Popup | Qt::FramelessWindowHint);
    setSelectionBehavior(QAbstractItemView::SelectRows);
    setSelectionMode(QAbstractItemView::SingleSelection);
    setTextElideMode(Qt::ElideMiddle);
    setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);

    setHeaderHidden(true);
    setRootIsDecorated(false);
}

SwitchChannelTreeView::~SwitchChannelTreeView() = default;

int SwitchChannelTreeView::sizeHintWidth() const
{
    return sizeHintForColumn(0);
}

void SwitchChannelTreeView::resizeColumnsToContents()
{
    resizeColumnToContents(0);
}

void SwitchChannelTreeView::keyReleaseEvent(QKeyEvent *event)
{
    if (event->key() == Qt::Key_Control) {
        Q_EMIT channelSelected(selectionModel()->currentIndex());
        event->accept();
        hide();
    } else {
        QTreeView::keyReleaseEvent(event);
    }
}

void SwitchChannelTreeView::keyPressEvent(QKeyEvent *event)
{
    if (event->key() == Qt::Key_Escape) {
        event->accept();
        hide();
    } else {
        QTreeView::keyPressEvent(event);
    }
}

void SwitchChannelTreeView::showEvent(QShowEvent *event)
{
    resizeColumnsToContents();
    reset();
    QTreeView::showEvent(event);
}
