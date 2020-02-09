/*
   Copyright (c) 2020 David Faure <faure@kde.org>

   This library is free software; you can redistribute it and/or modify
   it under the terms of the GNU Library General Public License as published
   by the Free Software Foundation; either version 2 of the License or
   ( at your option ) version 3 or, at the discretion of KDE e.V.
   ( which shall act as a proxy as in section 14 of the GPLv3 ), any later version.

   This library is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
   Library General Public License for more details.

   You should have received a copy of the GNU Library General Public License
   along with this library; see the file COPYING.LIB.  If not, write to
   the Free Software Foundation, Inc., 51 Franklin Street, Fifth Floor,
   Boston, MA 02110-1301, USA.
*/

#include "completionlistview.h"

#include <QApplication>
#include <QKeyEvent>
#include <QScreen>
#include <QScrollBar>

CompletionListView::CompletionListView()
    : QListView()
{
    setWindowFlag(Qt::Popup);
    hide();

    connect(this, &QListView::clicked, this, &CompletionListView::complete);
}

void CompletionListView::setTextWidget(QWidget *textWidget)
{
    mTextWidget = textWidget;
    const Qt::FocusPolicy origPolicy = textWidget->focusPolicy();
    setFocusPolicy(Qt::NoFocus);
    textWidget->setFocusPolicy(origPolicy);
    setFocusProxy(textWidget);
}

void CompletionListView::setModel(QAbstractItemModel *model)
{
    connect(model, &QAbstractItemModel::rowsInserted, this, &CompletionListView::slotCompletionAvailable);
    connect(model, &QAbstractItemModel::rowsRemoved, this, &CompletionListView::slotCompletionAvailable);
    QListView::setModel(model);
}

void CompletionListView::keyPressEvent(QKeyEvent *event)
{
    const int key = event->key();
    if (key == Qt::Key_Escape) {
        hide();
        event->accept();
        return;
    } else if (key == Qt::Key_Return
               || key == Qt::Key_Enter) {
        Q_EMIT complete(currentIndex());
        event->accept();
        return;
    } else if (key != Qt::Key_Down && key != Qt::Key_Up
               && key != Qt::Key_PageDown && key != Qt::Key_PageUp
               && key != Qt::Key_Home && key != Qt::Key_End
               && key != Qt::Key_Left && key != Qt::Key_Right) {
        // send keypresses to the linedit
        qApp->sendEvent(mTextWidget, event);
        return;
    }

    QListView::keyPressEvent(event);
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
#if QT_VERSION >= QT_VERSION_CHECK(5, 14, 0)
    const QRect screenRect = mTextWidget->screen()->availableGeometry();
#else
    const int screenNum = QApplication::desktop()->screenNumber(mTextWidget);
    auto *screen = QApplication::screens().value(screenNum);
    Q_ASSERT(screen);
    const QRect screenRect = screen->availableGeometry();
#endif
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

    //qDebug() << "showing at" << pos << "size" << w << "x" << h;
    setGeometry(pos.x(), pos.y(), w, h);

    if (!isVisible()) {
        if (!currentIndex().isValid()) {
            setCurrentIndex(model()->index(0, 0));
        }
        show();
    }
}
