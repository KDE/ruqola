/*
   SPDX-FileCopyrightText: 2025-2026 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "showimagegraphicsimagelabel.h"
using namespace Qt::Literals::StringLiterals;

#include <QApplication>
#include <QDrag>
#include <QMimeData>
#include <QMouseEvent>

ShowImageGraphicsImageLabel::ShowImageGraphicsImageLabel(QWidget *parent)
    : QLabel(parent)
{
}

ShowImageGraphicsImageLabel::~ShowImageGraphicsImageLabel() = default;

void ShowImageGraphicsImageLabel::setImagePath(const QString &path)
{
    mImagePath = path;
}

void ShowImageGraphicsImageLabel::mousePressEvent(QMouseEvent *event)
{
    if (event->button() == Qt::LeftButton && (event->modifiers() & Qt::ControlModifier)) {
        mDragStartPosition = event->pos();
        return;
    }
    mDragStartPosition = {};
    QLabel::mousePressEvent(event);
}

void ShowImageGraphicsImageLabel::mouseMoveEvent(QMouseEvent *event)
{
    if ((event->buttons() == Qt::LeftButton) && (event->modifiers() & Qt::ControlModifier)
        && (event->pos() - mDragStartPosition).manhattanLength() < QApplication::startDragDistance()) {
        QDrag *drag = new QDrag(this);
        QMimeData *mimeData = new QMimeData;
        mimeData->setUrls(QList<QUrl>{QUrl::fromLocalFile(mImagePath)});
        QByteArray itemData;
        QDataStream dataStream(&itemData, QIODevice::WriteOnly);
        dataStream << pixmap().toImage();
        mimeData->setData(u"application/x-dnditemdata"_s, itemData);

        drag->setMimeData(mimeData);
        drag->setPixmap(pixmap());
        drag->setHotSpot(event->pos());

        drag->exec(Qt::CopyAction | Qt::MoveAction);
        return;
    }
    QLabel::mouseMoveEvent(event);
}

#include "moc_showimagegraphicsimagelabel.cpp"
