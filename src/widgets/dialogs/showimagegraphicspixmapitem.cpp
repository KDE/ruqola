/*
   SPDX-FileCopyrightText: 2025 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/
#include "showimagegraphicspixmapitem.h"

#include <QApplication>
#include <QDrag>
#include <QGraphicsSceneMouseEvent>
#include <QIODevice>
#include <QMimeData>
#include <QWidget>

ShowImageGraphicsPixmapItem::ShowImageGraphicsPixmapItem(QGraphicsItem *parent)
    : QGraphicsPixmapItem(parent)
{
    setFlag(QGraphicsItem::ItemIsSelectable);
}

ShowImageGraphicsPixmapItem::~ShowImageGraphicsPixmapItem() = default;

void ShowImageGraphicsPixmapItem::mousePressEvent(QGraphicsSceneMouseEvent *event)
{
    if (event->button() == Qt::LeftButton) {
        dragStartPosition = event->pos();
    }
    QGraphicsPixmapItem::mousePressEvent(event);
}

void ShowImageGraphicsPixmapItem::mouseMoveEvent(QGraphicsSceneMouseEvent *event)
{
    if (!(event->buttons() & Qt::LeftButton)) {
        return;
    }
    if ((event->pos() - dragStartPosition).manhattanLength() < QApplication::startDragDistance()) {
        return;
    }
    QDrag *drag = new QDrag(event->widget());
    QMimeData *mimeData = new QMimeData;
    QByteArray itemData;
    QDataStream dataStream(&itemData, QIODevice::WriteOnly);
    dataStream << pixmap().toImage();
    mimeData->setData(QStringLiteral("application/x-dnditemdata"), itemData);

    drag->setMimeData(mimeData);
    drag->setPixmap(pixmap());
    drag->setHotSpot(event->pos().toPoint());

    drag->exec(Qt::CopyAction | Qt::MoveAction);
}
