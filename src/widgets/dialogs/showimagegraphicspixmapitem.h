/*
   SPDX-FileCopyrightText: 2025-2026 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/
#pragma once

#include "libruqolawidgets_private_export.h"
#include <QGraphicsPixmapItem>
class LIBRUQOLAWIDGETS_TESTS_EXPORT ShowImageGraphicsPixmapItem : public QGraphicsPixmapItem
{
public:
    explicit ShowImageGraphicsPixmapItem(QGraphicsItem *parent = nullptr);
    ~ShowImageGraphicsPixmapItem() override;
    void setImagePath(const QString &path);

protected:
    void mousePressEvent(QGraphicsSceneMouseEvent *event) override;
    void mouseMoveEvent(QGraphicsSceneMouseEvent *event) override;

private:
    QString mImagePath;
    QPointF mDragStartPosition;
};
