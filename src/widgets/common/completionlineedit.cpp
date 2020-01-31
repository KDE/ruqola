/*
   Copyright (c) 2020 Laurent Montel <montel@kde.org>

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

#include "completionlineedit.h"
#include "rocketchataccount.h"
#include "ruqola.h"

#include <QKeyEvent>
#include <QListView>
#include <QScreen>
#include <QScrollBar>

#if QT_VERSION < QT_VERSION_CHECK(5, 14, 0)
#include <QApplication>
#include <QDesktopWidget>
#endif

CompletionLineEdit::CompletionLineEdit(QWidget *parent)
    : QLineEdit(parent)
{
    setClearButtonEnabled(true);

    // QCompleter does the filtering itself... so we need to implement our own popup
    mCompletionListView = new QListView;
    mCompletionListView->setWindowFlag(Qt::Popup);
    const Qt::FocusPolicy origPolicy = focusPolicy();
    mCompletionListView->setFocusPolicy(Qt::NoFocus);
    setFocusPolicy(origPolicy);
    mCompletionListView->setFocusProxy(this);
    mCompletionListView->installEventFilter(this);

    mCompletionListView->hide();

    connect(mCompletionListView, &QListView::clicked, this, &CompletionLineEdit::complete);
}

CompletionLineEdit::~CompletionLineEdit()
{
    delete mCompletionListView;
}

void CompletionLineEdit::setCompletionModel(QAbstractItemModel *model)
{
    mCompletionListView->setModel(model);
    connect(model, &QAbstractItemModel::rowsInserted, this, &CompletionLineEdit::slotCompletionAvailable);
    connect(model, &QAbstractItemModel::rowsRemoved, this, &CompletionLineEdit::slotCompletionAvailable);
}

void CompletionLineEdit::keyPressEvent(QKeyEvent *e)
{
    switch (e->key()) {
    case Qt::Key_Escape:
        e->ignore();
        return;
    default:
        break;
    }
    QLineEdit::keyPressEvent(e);
}

bool CompletionLineEdit::eventFilter(QObject *watched, QEvent *ev)
{
    if (watched == mCompletionListView) {
        const QEvent::Type eventType = ev->type();
        if (eventType == QEvent::KeyPress) {
            QKeyEvent *kev = static_cast<QKeyEvent *>(ev);
            const int key = kev->key();
            if (key == Qt::Key_Escape) {
                mCompletionListView->hide();
                return true;
            } else if (key == Qt::Key_Return ||
                       key == Qt::Key_Enter) {
                Q_EMIT complete(mCompletionListView->currentIndex());
                return true;
            }
            event(ev);
        }
    }
    return QLineEdit::eventFilter(watched, ev);
}

void CompletionLineEdit::slotCompletionAvailable()
{
    const int rowCount = mCompletionListView->model()->rowCount();
    if (rowCount == 0) {
        mCompletionListView->hide();
        return;
    }
    const int maxVisibleItems = 15;

    // Not entirely unlike QCompletionPrivate::showPopup
#if QT_VERSION >= QT_VERSION_CHECK(5, 14, 0)
    const QRect screenRect = screen()->availableGeometry();
#else
    const int screenNum = QApplication::desktop()->screenNumber(this);
    auto *screen = QApplication::screens().value(screenNum);
    Q_ASSERT(screen);
    const QRect screenRect = screen->availableGeometry();
#endif
    int h = (mCompletionListView->sizeHintForRow(0) * qMin(maxVisibleItems, rowCount) + 3) + 3;
    QScrollBar *hsb = mCompletionListView->horizontalScrollBar();
    if (hsb && hsb->isVisible())
        h += mCompletionListView->horizontalScrollBar()->sizeHint().height();

    const int rh = height();
    QPoint pos = mapToGlobal(QPoint(0, height() - 2));
    int w = width();

    if (w > screenRect.width())
        w = screenRect.width();
    if ((pos.x() + w) > (screenRect.x() + screenRect.width()))
        pos.setX(screenRect.x() + screenRect.width() - w);
    if (pos.x() < screenRect.x())
        pos.setX(screenRect.x());

    int top = pos.y() - rh - screenRect.top() + 2;
    int bottom = screenRect.bottom() - pos.y();
    h = qMax(h, mCompletionListView->minimumHeight());
    if (h > bottom) {
        h = qMin(qMax(top, bottom), h);

        if (top > bottom)
            pos.setY(pos.y() - h - rh + 2);
    }

    //qDebug() << "showing at" << pos << "size" << w << "x" << h;
    mCompletionListView->setGeometry(pos.x(), pos.y(), w, h);

    if (!mCompletionListView->isVisible()) {
        if (!mCompletionListView->currentIndex().isValid())
            mCompletionListView->setCurrentIndex(mCompletionListView->model()->index(0, 0));
        mCompletionListView->show();
    }
}
