/*
   SPDX-FileCopyrightText: 2022-2024 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "switchchanneltreeviewmanager.h"
#include "model/switchchannelhistorymodel.h"
#include "rocketchataccount.h"
#include "ruqolawidgets_debug.h"
#include "switchchanneltreeview.h"
#include <QScrollBar>

SwitchChannelTreeViewManager::SwitchChannelTreeViewManager(QObject *parent)
    : QObject{parent}
    , mSwitcherChannelTreeView(new SwitchChannelTreeView(nullptr)) // TODO use parent ???
{
    connect(mSwitcherChannelTreeView, &SwitchChannelTreeView::pressed, this, &SwitchChannelTreeViewManager::switchToCollectionClicked);
    connect(mSwitcherChannelTreeView, &SwitchChannelTreeView::channelSelected, this, &SwitchChannelTreeViewManager::activateChannel);
}

SwitchChannelTreeViewManager::~SwitchChannelTreeViewManager()
{
    delete mSwitcherChannelTreeView;
}

void SwitchChannelTreeViewManager::addActions(const QList<QAction *> &lst)
{
    // Make sure that actions works when mSwitcherChannelTreeView is show.
    mSwitcherChannelTreeView->addActions(lst);
}

void SwitchChannelTreeViewManager::activateChannel(const QModelIndex &index)
{
    if (!index.isValid()) {
        return;
    }
    const QString identifier = index.data(SwitchChannelHistoryModel::SwitchChannelHistoryRoles::Identifier).toString();
    Q_EMIT switchToChannel(identifier);

    mSwitcherChannelTreeView->hide();
}

void SwitchChannelTreeViewManager::switchToCollectionClicked(const QModelIndex &index)
{
    mSwitcherChannelTreeView->selectionModel()->select(index, QItemSelectionModel::ClearAndSelect);
    activateChannel(index);
}

QWidget *SwitchChannelTreeViewManager::parentWidget() const
{
    return mParentWidget;
}

void SwitchChannelTreeViewManager::setParentWidget(QWidget *newParentWidget)
{
    mParentWidget = newParentWidget;
}

void SwitchChannelTreeViewManager::selectChannel(const int from, const int to)
{
    if (!mCurrentRocketChatAccount) {
        return;
    }
    if (mCurrentRocketChatAccount->switchChannelHistoryModel()->rowCount() == 0) {
        return;
    }
    QModelIndex index;
    const int step = from < to ? 1 : -1;
    if (!mSwitcherChannelTreeView->isVisible()) {
        updateViewGeometry();
        index = mCurrentRocketChatAccount->switchChannelHistoryModel()->index(from + step, 0);
        if (!index.isValid()) {
            index = mCurrentRocketChatAccount->switchChannelHistoryModel()->index(0, 0);
        }
        mSwitcherChannelTreeView->show();
        mSwitcherChannelTreeView->setFocus();
    } else {
        int newRow = mSwitcherChannelTreeView->selectionModel()->currentIndex().row() + step;
        if (newRow == to + step) {
            newRow = from;
        }
        index = mCurrentRocketChatAccount->switchChannelHistoryModel()->index(newRow, 0);
    }

    mSwitcherChannelTreeView->selectionModel()->select(index, QItemSelectionModel::Rows | QItemSelectionModel::ClearAndSelect);
    mSwitcherChannelTreeView->selectionModel()->setCurrentIndex(index, QItemSelectionModel::SelectCurrent | QItemSelectionModel::Rows);
}

RocketChatAccount *SwitchChannelTreeViewManager::currentRocketChatAccount() const
{
    return mCurrentRocketChatAccount;
}

void SwitchChannelTreeViewManager::setCurrentRocketChatAccount(RocketChatAccount *newCurrentRocketChatAccount)
{
    if (mCurrentRocketChatAccount != newCurrentRocketChatAccount) {
        mCurrentRocketChatAccount = newCurrentRocketChatAccount;
        mSwitcherChannelTreeView->setCurrentRocketChatAccount(mCurrentRocketChatAccount);
    }
}

SwitchChannelTreeView *SwitchChannelTreeViewManager::switchChannelTreeView() const
{
    return mSwitcherChannelTreeView;
}

void SwitchChannelTreeViewManager::selectForward()
{
    selectChannel(0, mCurrentRocketChatAccount->switchChannelHistoryModel()->rowCount() - 1);
}

void SwitchChannelTreeViewManager::selectBackward()
{
    selectChannel(mCurrentRocketChatAccount->switchChannelHistoryModel()->rowCount() - 1, 0);
}

void SwitchChannelTreeViewManager::updateViewGeometry()
{
    QWidget *window = mParentWidget ? mParentWidget->window() : nullptr;
    if (window) {
        const QSize centralSize = window->size();

        const QSize viewMaxSize(centralSize.width() * 3 / 4, centralSize.height() * 3 / 4);

        const int rowHeight = mSwitcherChannelTreeView->sizeHintForRow(0);
        const int frameWidth = mSwitcherChannelTreeView->frameWidth();
        const QSize viewSize(
            std::min(mSwitcherChannelTreeView->sizeHintWidth() + 2 * frameWidth + mSwitcherChannelTreeView->verticalScrollBar()->width(), viewMaxSize.width()),
            std::min(std::max(rowHeight * mCurrentRocketChatAccount->switchChannelHistoryModel()->rowCount() + 2 * frameWidth, rowHeight * 6),
                     viewMaxSize.height()));

        // Position should be central over the editor area, so map to global from
        // parent of central widget since the view is positioned in global coords
        const QPoint centralWidgetPos = window->parentWidget() ? window->mapToGlobal(window->pos()) : window->pos();
        const int xPos = std::max(0, centralWidgetPos.x() + (centralSize.width() - viewSize.width()) / 2);
        const int yPos = std::max(0, centralWidgetPos.y() + (centralSize.height() - viewSize.height()) / 2);

        mSwitcherChannelTreeView->setFixedSize(viewSize);
        mSwitcherChannelTreeView->move(xPos, yPos);
    } else {
        qCWarning(RUQOLAWIDGETS_LOG) << "Problem mParentWidget is null! it's a bug";
    }
}

#include "moc_switchchanneltreeviewmanager.cpp"
