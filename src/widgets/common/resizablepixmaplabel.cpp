/*
   SPDX-FileCopyrightText: 2025 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "resizablepixmaplabel.h"
#include <QDebug>
#include <QScreen>

ResizablePixmapLabel::ResizablePixmapLabel(QWidget *parent)
    : QLabel(parent)
{
}

ResizablePixmapLabel::~ResizablePixmapLabel() = default;

void ResizablePixmapLabel::setCurrentPixmap(const QPixmap &pix)
{
    mPix = pix;
    qDebug() << " mPix " << mPix;
    const QSize s = QSize(200, 200) * screen()->devicePixelRatio();
    const QPixmap p = mPix.scaled(s, Qt::KeepAspectRatio, Qt::SmoothTransformation);
    setMinimumSize(s);
    setPixmap(p);
}

void ResizablePixmapLabel::resizeEvent(QResizeEvent *event)
{
    QLabel::resizeEvent(event);
    if (!pixmap().isNull()) {
        const QPixmap pixmap = mPix.scaled(QSize(qMin(size().width(), mPix.size().width()), qMin(size().height(), mPix.size().height())),
                                           Qt::KeepAspectRatio,
                                           Qt::SmoothTransformation);
        setPixmap(pixmap);
        // qDebug() << " pixmap.size " << pixmap.size();
        // qDebug() << " mPix.size " << mPix.size();
    }
}

#include "moc_resizablepixmaplabel.cpp"
