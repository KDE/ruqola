/*
   SPDX-FileCopyrightText: 2022-2023 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "messagelistviewbase.h"
#include "model/messagesmodel.h"
#include "room/plugins/plugintext.h"
#include "room/plugins/plugintextinterface.h"
#include "room/textpluginmanager.h"
#include <QAbstractItemModel>
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
    setMouseTracking(true);

    const QVector<PluginText *> plugins = TextPluginManager::self()->pluginsList();
    for (PluginText *plugin : plugins) {
        connect(plugin, &PluginText::errorMessage, this, &MessageListViewBase::errorMessage);
        connect(plugin, &PluginText::successMessage, this, &MessageListViewBase::successMessage);
        mPluginTextInterface.append(plugin->createInterface(this));
    }
}

MessageListViewBase::~MessageListViewBase()
{
    qDeleteAll(mPluginTextInterface);
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

void MessageListViewBase::handleMouseEvent(QMouseEvent *event)
{
    const QPersistentModelIndex index = indexAt(event->pos());
    if (index.isValid()) {
        // When the cursor hovers another message, hide/show the reaction icon accordingly
        if (mCurrentIndex != index) {
            if (mCurrentIndex.isValid()) {
                auto lastModel = const_cast<QAbstractItemModel *>(mCurrentIndex.model());
                lastModel->setData(mCurrentIndex, false, MessagesModel::HoverHighLight);
            }
            mCurrentIndex = index;
            auto model = const_cast<QAbstractItemModel *>(mCurrentIndex.model());
            model->setData(mCurrentIndex, true, MessagesModel::HoverHighLight);
        }

        QStyleOptionViewItem options = listViewOptions();
        options.rect = visualRect(mCurrentIndex);
        if (mouseEvent(event, options, mCurrentIndex)) {
            update(mCurrentIndex);
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

void MessageListViewBase::leaveEvent(QEvent *event)
{
    if (mCurrentIndex.isValid()) {
        auto lastModel = const_cast<QAbstractItemModel *>(mCurrentIndex.model());
        lastModel->setData(mCurrentIndex, false, MessagesModel::HoverHighLight);
        mCurrentIndex = QPersistentModelIndex();
    }
    QListView::leaveEvent(event);
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

void MessageListViewBase::addTextPlugins(QMenu *menu, const QString &selectedText)
{
    for (PluginTextInterface *interface : std::as_const(mPluginTextInterface)) {
        interface->setSelectedText(selectedText);
        interface->addAction(menu);
    }
}

#include "moc_messagelistviewbase.cpp"
