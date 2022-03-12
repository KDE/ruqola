/*
   SPDX-FileCopyrightText: 2022 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "uploadfileprogressstatuslistwidget.h"
#include "uploadfileprogressstatuswidget.h"
#include <QScrollBar>
#include <QVBoxLayout>

UploadFileProgressStatusListWidget::UploadFileProgressStatusListWidget(QWidget *parent)
    : QScrollArea{parent}
    , mBigBox(new QWidget(this))
{
    setFrameStyle(NoFrame);
    auto mBigBoxVBoxLayout = new QVBoxLayout(mBigBox);
    mBigBoxVBoxLayout->setContentsMargins(0, 0, 0, 0);
    setWidget(mBigBox);
    setWidgetResizable(true);
    setSizePolicy(QSizePolicy::Preferred, QSizePolicy::Fixed);
}

UploadFileProgressStatusListWidget::~UploadFileProgressStatusListWidget()
{
}

void UploadFileProgressStatusListWidget::resizeEvent(QResizeEvent *event)
{
    // Tell the layout in the parent (progressdialog) that our size changed
    updateGeometry();
    QScrollArea::resizeEvent(event);
}

QSize UploadFileProgressStatusListWidget::sizeHint() const
{
    return minimumSizeHint();
}

QSize UploadFileProgressStatusListWidget::minimumSizeHint() const
{
    const int f = 2 * frameWidth();
    // Make room for a vertical scrollbar in all cases, to avoid a horizontal one
    const int vsbExt = verticalScrollBar()->sizeHint().width();
    const int minw = topLevelWidget()->width() / 3;
    const int maxh = topLevelWidget()->height() / 2;
    QSize sz(mBigBox->minimumSizeHint());
    sz.setWidth(qMax(sz.width(), minw) + f + vsbExt);
    sz.setHeight(qMin(sz.height(), maxh) + f);
    return sz;
}

void UploadFileProgressStatusListWidget::slotLayoutFirstItem()
{
    // This slot is called whenever a TransactionItem is deleted, so this is a
    // good place to call updateGeometry(), so our parent takes the new size
    // into account and resizes.
    updateGeometry();
#if 0
    /*
     The below relies on some details in Qt's behaviour regarding deleting
     objects. This slot is called from the destroyed signal of an item just
     going away. That item is at that point still in the  list of children, but
     since the vtable is already gone, it will have type QObject. The first
     one with both the right name and the right class therefore is what will
     be the first item very shortly. That's the one we want to remove the
     hline for.
    */
    auto *ti = mBigBox->findChild<KPIM::TransactionItem *>(QStringLiteral("TransactionItem"));
    if (ti) {
        ti->hideHLine();
    }
#endif
}

UploadFileProgressStatusWidget *UploadFileProgressStatusListWidget::addProgressStatusWidget(int identifier, bool first)
{
    auto ti = new UploadFileProgressStatusWidget(mBigBox);
    ti->setIdentifier(identifier);
    mBigBox->layout()->addWidget(ti);

    resize(mBigBox->width(), mBigBox->height());

    mUploadItems.insert(identifier, ti);
    return ti;
}

void UploadFileProgressStatusListWidget::removeUploadFileProgressStatusWidget()
{
    // TODO
}
