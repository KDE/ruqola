/*
   SPDX-FileCopyrightText: 2022-2024 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/
#include "switchchanneltreeview.h"
#include "model/switchchannelhistorymodel.h"
#include "rocketchataccount.h"
#include "switchchanneldelegate.h"
#include <QKeyEvent>

SwitchChannelTreeView::SwitchChannelTreeView(QWidget *parent)
    : QTreeView(parent)
    , mSwitchChannelDelegate(new SwitchChannelDelegate(this))
{
    setWindowFlags(Qt::Popup | Qt::FramelessWindowHint);
    setSelectionBehavior(QAbstractItemView::SelectRows);
    setSelectionMode(QAbstractItemView::SingleSelection);
    setTextElideMode(Qt::ElideMiddle);
    setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);

    setHeaderHidden(true);
    setRootIsDecorated(false);
    mSwitchChannelDelegate->setObjectName(QStringLiteral("mSwitchChannelDelegate"));
    setItemDelegate(mSwitchChannelDelegate);
}

SwitchChannelTreeView::~SwitchChannelTreeView() = default;

void SwitchChannelTreeView::setCurrentRocketChatAccount(RocketChatAccount *newCurrentRocketChatAccount)
{
    mSwitchChannelDelegate->setCurrentRocketChatAccount(newCurrentRocketChatAccount);
    setModel(newCurrentRocketChatAccount->switchChannelHistoryModel());
}

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

#include "moc_switchchanneltreeview.cpp"
