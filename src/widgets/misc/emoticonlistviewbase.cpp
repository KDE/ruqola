/*
   SPDX-FileCopyrightText: 2022 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/
#include "emoticonlistviewbase.h"
#include "ruqolaglobalconfig.h"
#include "utils.h"
#include <QKeyEvent>

EmoticonListViewBase::EmoticonListViewBase(QWidget *parent)
    : QListView(parent)
{
    setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    setSelectionMode(QAbstractItemView::SingleSelection);
    setMouseTracking(true);
    setDragEnabled(false);
    setIconSize(QSize(22, 22));
    mFontSize = RuqolaGlobalConfig::self()->emojiMenuFontSize();

    applyFontSize();
}

EmoticonListViewBase::~EmoticonListViewBase() = default;

void EmoticonListViewBase::keyPressEvent(QKeyEvent *event)
{
    const bool isControlClicked = event->modifiers() & Qt::ControlModifier;
    int fontSize = mFontSize;
    if (isControlClicked) {
        if (event->key() == Qt::Key_Plus) {
            Q_EMIT fontSizeChanged(++fontSize);
        } else if (event->key() == Qt::Key_Minus) {
            Q_EMIT fontSizeChanged(--fontSize);
        }
    } else {
        QListView::keyPressEvent(event);
    }
}

void EmoticonListViewBase::wheelEvent(QWheelEvent *e)
{
    int fontSize = mFontSize;
    if (e->modifiers() == Qt::ControlModifier) {
        const int y = e->angleDelta().y();
        if (y < 0) {
            Q_EMIT fontSizeChanged(--fontSize);
        } else if (y > 0) {
            Q_EMIT fontSizeChanged(++fontSize);
        } // else: y == 0 => horizontal scroll => do not handle
    } else {
        QListView::wheelEvent(e);
    }
}

void EmoticonListViewBase::setFontSize(int newFontSize)
{
    if (newFontSize < 10 || newFontSize > 30) {
        return;
    }
    if (mFontSize != newFontSize) {
        mFontSize = newFontSize;
        applyFontSize();
    }
}

void EmoticonListViewBase::applyFontSize()
{
    QFont f = font();
    f.setPointSize(mFontSize);
    f.setFamily(Utils::emojiFontName());
    setFont(f);
}
