/*
   SPDX-FileCopyrightText: 2025 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "showimagegraphicsimagelabel.h"

#include <QApplication>
#include <QDrag>
#include <QMimeData>
#include <QMouseEvent>

ShowImageGraphicsImageLabel::ShowImageGraphicsImageLabel(RocketChatAccount *account, QWidget *parent)
    : QLabel(parent)
    , mRocketChatAccount(account)
{
}

ShowImageGraphicsImageLabel::~ShowImageGraphicsImageLabel() = default;

void ShowImageGraphicsImageLabel::mousePressEvent(QMouseEvent *event)
{
    if (event->button() == Qt::LeftButton) {
        mDragStartPosition = event->pos();
    }
}

void ShowImageGraphicsImageLabel::mouseMoveEvent(QMouseEvent *event)
{
    if (!(event->buttons() & Qt::LeftButton)) {
        return;
    }
    if ((event->pos() - mDragStartPosition).manhattanLength() < QApplication::startDragDistance()) {
        return;
    }

    QDrag *drag = new QDrag(this);
    QMimeData *mimeData = new QMimeData;

    QPixmap pixmap = this->pixmap();
    mimeData->setImageData(pixmap);

    drag->setMimeData(mimeData);
    drag->setPixmap(pixmap);
    drag->setHotSpot(event->pos());

    drag->exec(Qt::CopyAction | Qt::MoveAction);
}

#include "moc_showimagegraphicsimagelabel.cpp"
