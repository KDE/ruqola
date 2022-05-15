/*
   SPDX-FileCopyrightText: 2022 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "messagelistviewbase.h"
#include <QApplication>
#include <QMouseEvent>
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

MessageListViewBase::~MessageListViewBase() = default;

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

void MessageListViewBase::handleMouseEvent(QMouseEvent *event)
{
    const QPersistentModelIndex index = indexAt(event->pos());
    if (index.isValid()) {
        QStyleOptionViewItem options = listViewOptions();
        options.rect = visualRect(index);
        if (mouseEvent(event, options, index)) {
            update(index);
        }
    }
}

void MessageListViewBase::mouseReleaseEvent(QMouseEvent *event)
{
    handleMouseEvent(event);
}

void MessageListViewBase::mouseDoubleClickEvent(QMouseEvent *event)
{
    handleMouseEvent(event);
}

void MessageListViewBase::mousePressEvent(QMouseEvent *event)
{
    mPressedPosition = event->pos();
    handleMouseEvent(event);
}

void MessageListViewBase::mouseMoveEvent(QMouseEvent *event)
{
    // Drag support
    const int distance = (event->pos() - mPressedPosition).manhattanLength();
    if (distance > QApplication::startDragDistance()) {
        mPressedPosition = {};
        const QPersistentModelIndex index = indexAt(event->pos());
        if (index.isValid()) {
            QStyleOptionViewItem options = listViewOptions();
            options.rect = visualRect(index);
            if (maybeStartDrag(event, options, index)) {
                return;
            }
        }
    }
    handleMouseEvent(event);
}

QStyleOptionViewItem MessageListViewBase::listViewOptions() const
{
#if QT_VERSION < QT_VERSION_CHECK(6, 0, 0)
    return QListView::viewOptions();
#else
    QStyleOptionViewItem option;
    initViewItemOption(&option);
    return option;
#endif
}

bool MessageListViewBase::maybeStartDrag(QMouseEvent *event, const QStyleOptionViewItem &option, const QModelIndex &index)
{
    Q_UNUSED(event);
    Q_UNUSED(option);
    Q_UNUSED(index);
    return false;
}

bool MessageListViewBase::mouseEvent(QMouseEvent *event, const QStyleOptionViewItem &option, const QModelIndex &index)
{
    Q_UNUSED(event);
    Q_UNUSED(option);
    Q_UNUSED(index);
    return false;
}
