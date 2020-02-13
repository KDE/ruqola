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

#include "flowlayout.h"

#include <QWidget>
#include <QStyle>


static int smartSpacing(QObject *parent, QStyle::PixelMetric pm)
{
    if (!parent) {
        return -1;
    } else if (parent->isWidgetType()) {
        QWidget *pw = static_cast<QWidget *>(parent);
        return pw->style()->pixelMetric(pm, nullptr, pw);
    } else {
        return static_cast<QLayout *>(parent)->spacing();
    }
}

FlowLayout::FlowLayout(QWidget *parent)
    : QLayout(parent)
{
}

FlowLayout::~FlowLayout()
{
    clear();
}

int FlowLayout::horizontalSpacing() const
{
    if (mHorizontalSpacing >= 0) {
        return mHorizontalSpacing;
    } else {
        return smartSpacing(parent(), QStyle::PM_LayoutHorizontalSpacing);
    }
}

void FlowLayout::setHorizontalSpacing(int horizontalSpacing)
{
    if (mHorizontalSpacing != horizontalSpacing) {
        mHorizontalSpacing = horizontalSpacing;
        invalidate();
    }
}

int FlowLayout::verticalSpacing() const
{
    if (mVerticalSpacing >= 0) {
        return mVerticalSpacing;
    } else {
        return smartSpacing(parent(), QStyle::PM_LayoutVerticalSpacing);
    }
}

void FlowLayout::setVerticalSpacing(int verticalSpacing)
{
    if (mVerticalSpacing != verticalSpacing) {
        mVerticalSpacing = verticalSpacing;
        invalidate();
    }
}

QSize FlowLayout::sizeHint() const
{
    return minimumSize();
}

QSize FlowLayout::minimumSize() const
{
    const QMargins margins = contentsMargins();
    QSize size;

    for (const QLayoutItem *item : mItems) {
        size = size.expandedTo(item->minimumSize());
    }

    size += QSize(margins.left() + margins.right(), margins.top() + margins.bottom());
    return size;
}

void FlowLayout::addItem(QLayoutItem *item)
{
    Q_ASSERT(!mItems.contains(item));
    mItems.append(item);
    invalidate();
}

QLayoutItem *FlowLayout::itemAt(int index) const
{
    if (index >= 0 && index < mItems.count()) {
        return mItems[index];
    }

    return nullptr;
}

QLayoutItem *FlowLayout::takeAt(int index)
{
    if (index >= 0 && index < mItems.count()) {
        auto *it = mItems.takeAt(index);
        invalidate();
        return it;
    }

    return nullptr;
}

int FlowLayout::count() const
{
    return mItems.count();
}

Qt::Orientations FlowLayout::expandingDirections() const
{
    return 0;
}

bool FlowLayout::hasHeightForWidth() const
{
    return true;
}

int FlowLayout::heightForWidth(int width) const
{
    return doFlow(QRect(0, 0, width, 0), false);
}

void FlowLayout::setGeometry(const QRect &rect)
{
    QLayout::setGeometry(rect);
    doFlow(rect, true);
}

void FlowLayout::clear()
{
    while (QLayoutItem *item = takeAt(0)) {
        delete item;
    }
}

void FlowLayout::clearAndDeleteWidgets()
{
    while (QLayoutItem *item = takeAt(0)) {
        delete item->widget();
        delete item;
    }
}

int FlowLayout::doFlow(const QRect &rect, bool effective) const
{
    const QMargins margins = contentsMargins();
    const QRect effectiveRect = rect.adjusted(margins.left(), margins.top(), -margins.right(), -margins.bottom());
    int x = effectiveRect.x();
    int y = effectiveRect.y();
    int heighest = 0;

    for (QLayoutItem *item : mItems) {
        const QWidget *widget = item->widget();

        if (!widget->isVisibleTo(parentWidget())) {
            continue;
        }

        int hSpacing = horizontalSpacing();
        int vSpacing = verticalSpacing();

        if (hSpacing == -1) {
            hSpacing = widget->style()->layoutSpacing(QSizePolicy::Frame, QSizePolicy::Frame, Qt::Horizontal);
        }

        if (vSpacing == -1) {
            vSpacing = widget->style()->layoutSpacing(QSizePolicy::Frame, QSizePolicy::Frame, Qt::Vertical);
        }

        int widgetXPos = x + item->sizeHint().width() + hSpacing;

        if (widgetXPos - hSpacing > effectiveRect.right() && heighest > 0) {
            x = effectiveRect.x();
            y += heighest + vSpacing;
            widgetXPos = x + item->sizeHint().width() + hSpacing;
            heighest = 0;
        }

        if (effective) {
            item->setGeometry(QRect(QPoint(x, y), item->sizeHint()));
        }

        x = widgetXPos;
        heighest = qMax(heighest, item->sizeHint().height());
    }

    return y + heighest - rect.y() + margins.bottom();
}
