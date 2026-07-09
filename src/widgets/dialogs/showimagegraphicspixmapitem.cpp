/*
   SPDX-FileCopyrightText: 2025-2026 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/
#include "showimagegraphicspixmapitem.h"

#include <QApplication>
#include <QDrag>
#include <QGraphicsSceneMouseEvent>
#include <QIODevice>
#include <QMimeData>
#include <QUrl>
#include <QWidget>

using namespace Qt::Literals::StringLiterals;
ShowImageGraphicsPixmapItem::ShowImageGraphicsPixmapItem(QGraphicsItem *parent)
    : QGraphicsPixmapItem(parent)
{
}

ShowImageGraphicsPixmapItem::~ShowImageGraphicsPixmapItem() = default;

void ShowImageGraphicsPixmapItem::setImagePath(const QString &path)
{
    mImagePath = path;
}

void ShowImageGraphicsPixmapItem::mousePressEvent(QGraphicsSceneMouseEvent *event)
{
    if (event->button() == Qt::LeftButton && (event->modifiers() & Qt::ControlModifier)) {
        mDragStartPosition = event->pos();
        return;
    }
    mDragStartPosition = {};
    QGraphicsPixmapItem::mousePressEvent(event);
}

void ShowImageGraphicsPixmapItem::mouseMoveEvent(QGraphicsSceneMouseEvent *event)
{
    if ((event->buttons() == Qt::LeftButton) && (event->modifiers() & Qt::ControlModifier)
        && (event->pos() - mDragStartPosition).manhattanLength() < QApplication::startDragDistance()) {
        QDrag *drag = new QDrag(event->widget());
        QMimeData *mimeData = new QMimeData;
        mimeData->setUrls(QList<QUrl>{QUrl::fromLocalFile(mImagePath)});
        QByteArray itemData;
        QDataStream dataStream(&itemData, QIODevice::WriteOnly);
        dataStream << pixmap().toImage();
        mimeData->setData(u"application/x-dnditemdata"_s, itemData);

        drag->setMimeData(mimeData);
        // TODO verify it
        drag->setPixmap(pixmap().scaled(200, 200, Qt::KeepAspectRatio));

        drag->exec(Qt::CopyAction | Qt::MoveAction);
        return;
    }
    QGraphicsPixmapItem::mouseMoveEvent(event);
}
