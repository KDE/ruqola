/*
   SPDX-FileCopyrightText: 2022 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "messagelistviewbase.h"
#include <QScrollBar>

MessageListViewBase::MessageListViewBase(QWidget *parent)
    : QListView(parent)
{
    setSelectionMode(QAbstractItemView::NoSelection);
    setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    setVerticalScrollMode(QAbstractItemView::ScrollPerPixel); // nicer in case of huge messages
    setWordWrap(true); // so that the delegate sizeHint is called again when the width changes
    // only the lineedit takes focus
    setFocusPolicy(Qt::NoFocus);
    scrollToBottom();
}

MessageListViewBase::~MessageListViewBase()
{
}

void MessageListViewBase::resizeEvent(QResizeEvent *ev)
{
    QListView::resizeEvent(ev);

    // Fix not being really at bottom when the view gets reduced by the header widget becoming taller
    checkIfAtBottom();
    maybeScrollToBottom(); // this forces a layout in QAIV, which then changes the vbar max value
    updateVerticalPageStep();
}

void MessageListViewBase::checkIfAtBottom()
{
    auto *vbar = verticalScrollBar();
    mAtBottom = vbar->value() == vbar->maximum();
}

void MessageListViewBase::maybeScrollToBottom()
{
    if (mAtBottom) {
        scrollToBottom();
    }
}

void MessageListViewBase::updateVerticalPageStep()
{
    verticalScrollBar()->setPageStep(viewport()->height() * 3 / 4);
}
