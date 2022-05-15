/*
   SPDX-FileCopyrightText: 2022 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "uploadfileprogressstatuslistwidget.h"
#include "ruqolawidgets_debug.h"
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

UploadFileProgressStatusListWidget::~UploadFileProgressStatusListWidget() = default;

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
    updateGeometry();
}

void UploadFileProgressStatusListWidget::addProgressStatusWidget(int identifier)
{
    auto ti = new UploadFileProgressStatusWidget(mBigBox);
    ti->setIdentifier(identifier);
    mBigBox->layout()->addWidget(ti);

    resize(mBigBox->width(), mBigBox->height());

    mUploadItems.insert(identifier, ti);
    connect(ti, &UploadFileProgressStatusWidget::cancelUpload, this, &UploadFileProgressStatusListWidget::cancelUpload);
}

void UploadFileProgressStatusListWidget::removeUploadFileProgressStatusWidget(int identifier)
{
    UploadFileProgressStatusWidget *item = mUploadItems.take(identifier);
    if (item) {
        item->deleteLater();
    }
}

void UploadFileProgressStatusListWidget::uploadProgress(const RocketChatRestApi::UploadFileJob::UploadStatusInfo &info,
                                                        int identifier,
                                                        const QString &accountName)
{
    UploadFileProgressStatusWidget *item = mUploadItems.value(identifier);
    if (item) {
        if (info.bytesSent > 0 && info.bytesTotal > 0) {
            item->setVisible(true);
            item->setUploadFileName(info.fileName + QStringLiteral(" (%1)").arg(accountName));
            item->setValue(static_cast<int>((info.bytesSent * 100) / info.bytesTotal));
            setVisible(true);
        } else {
            connect(item, &QObject::destroyed, this, &UploadFileProgressStatusListWidget::slotLayoutFirstItem);
            item->setVisible(false);
            mUploadItems.remove(identifier);
            item->deleteLater();
            if (mUploadItems.isEmpty()) {
                setVisible(false);
            }
        }
    } else {
        qCWarning(RUQOLAWIDGETS_LOG) << "identifier " << identifier << " not found.";
    }
}
