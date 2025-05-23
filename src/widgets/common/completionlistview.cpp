/*
   SPDX-FileCopyrightText: 2020 David Faure <faure@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "completionlistview.h"

#include <QApplication>
#include <QDebug>
#include <QKeyEvent>
#include <QScreen>
#include <QScrollBar>

CompletionListView::CompletionListView()

{
    setUniformItemSizes(true);
    setWindowFlag(Qt::Popup);
    hide();

    connect(this, &QListView::clicked, this, &CompletionListView::complete);
}

CompletionListView::~CompletionListView() = default;

void CompletionListView::setTextWidget(QWidget *textWidget)
{
    mTextWidget = textWidget;
    const Qt::FocusPolicy origPolicy = textWidget->focusPolicy();
    setFocusPolicy(Qt::NoFocus);
    textWidget->setFocusPolicy(origPolicy);
    setFocusProxy(textWidget);
}

// Typically called with one of the 3 models returned by InputTextManager:
// inputCompleterModel() for users and channels, emojiCompleterModel(), or commandModel()
void CompletionListView::setModel(QAbstractItemModel *model)
{
    QAbstractItemModel *oldModel = QListView::model();
    if (model != oldModel) {
        if (oldModel) {
            disconnect(oldModel, &QAbstractItemModel::rowsInserted, this, &CompletionListView::slotCompletionAvailable);
            disconnect(oldModel, &QAbstractItemModel::rowsRemoved, this, &CompletionListView::slotCompletionAvailable);
        }
        connect(model, &QAbstractItemModel::rowsInserted, this, &CompletionListView::slotCompletionAvailable);
        connect(model, &QAbstractItemModel::rowsRemoved, this, &CompletionListView::slotCompletionAvailable);
        QListView::setModel(model);
    }
}

void CompletionListView::keyPressEvent(QKeyEvent *event)
{
    const int key = event->key();
    if (key == Qt::Key_Escape) {
        hide();
        event->accept();
        return;
    } else if (key == Qt::Key_Return || key == Qt::Key_Enter) {
        Q_EMIT complete(currentIndex());
        event->accept();
        return;
    } else if ((event->modifiers() & Qt::AltModifier) && (key == Qt::Key_Down || key == Qt::Key_Up)) {
        hide();
        // send keypresses to the linedit
        qApp->sendEvent(mTextWidget, event);
        return;
    } else if (key != Qt::Key_Down && key != Qt::Key_Up && key != Qt::Key_PageDown && key != Qt::Key_PageUp && key != Qt::Key_Home && key != Qt::Key_End
               && key != Qt::Key_Left && key != Qt::Key_Right) {
        // send keypresses to the linedit
        qApp->sendEvent(mTextWidget, event);
        return;
    }

    QListView::keyPressEvent(event);
}

bool CompletionListView::event(QEvent *event)
{
    // QAbstractScrollArea::event doesn't call QWidget::mousePressEvent, which leads to
    // https://bugs.kde.org/show_bug.cgi?id=434473 when making the scrollarea a Qt::Popup
    if (event->type() == QEvent::MouseButtonPress) {
        QWidget::mousePressEvent(static_cast<QMouseEvent *>(event));
    }
    return QListView::event(event);
}

void CompletionListView::slotCompletionAvailable()
{
    const int rowCount = model()->rowCount();
    if (rowCount == 0) {
        hide();
        return;
    }
    const int maxVisibleItems = 15;

    // Not entirely unlike QCompletionPrivate::showPopup
    const QRect screenRect = mTextWidget->screen()->availableGeometry();
    int h = (sizeHintForRow(0) * qMin(maxVisibleItems, rowCount) + 3) + 3;
    QScrollBar *hsb = horizontalScrollBar();
    if (hsb && hsb->isVisible()) {
        h += horizontalScrollBar()->sizeHint().height();
    }

    const int rh = mTextWidget->height();
    QPoint pos = mTextWidget->mapToGlobal(QPoint(0, rh - 2));
    int w = mTextWidget->width();

    if (w > screenRect.width()) {
        w = screenRect.width();
    }
    if ((pos.x() + w) > (screenRect.x() + screenRect.width())) {
        pos.setX(screenRect.x() + screenRect.width() - w);
    }
    if (pos.x() < screenRect.x()) {
        pos.setX(screenRect.x());
    }

    int top = pos.y() - rh - screenRect.top() + 2;
    int bottom = screenRect.bottom() - pos.y();
    h = qMax(h, minimumHeight());
    if (h > bottom) {
        h = qMin(qMax(top, bottom), h);

        if (top > bottom) {
            pos.setY(pos.y() - h - rh + 2);
        }
    }

    // qDebug() << "showing at" << pos << "size" << w << "x" << h;
    setGeometry(pos.x(), pos.y(), w, h);

    if (!isVisible()) {
        if (!currentIndex().isValid()) {
            setCurrentIndex(model()->index(0, 0));
        }
        show();
    }
}

#include "moc_completionlistview.cpp"
