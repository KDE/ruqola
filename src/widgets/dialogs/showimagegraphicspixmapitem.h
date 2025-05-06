/*
   SPDX-FileCopyrightText: 2025 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/
#pragma once

#include "libruqolawidgets_private_export.h"
#include <QGraphicsPixmapItem>
class RocketChatAccount;
class LIBRUQOLAWIDGETS_TESTS_EXPORT ShowImageGraphicsPixmapItem : public QGraphicsPixmapItem
{
public:
    explicit ShowImageGraphicsPixmapItem(RocketChatAccount *account, QGraphicsItem *parent = nullptr);
    ~ShowImageGraphicsPixmapItem() override;

protected:
    void mousePressEvent(QGraphicsSceneMouseEvent *event) override;
    void mouseMoveEvent(QGraphicsSceneMouseEvent *event) override;

private:
    QPointF dragStartPosition;
    RocketChatAccount *const mRocketChatAccount;
};
