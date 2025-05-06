/*
   SPDX-FileCopyrightText: 2025 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#pragma once
#include "libruqolawidgets_private_export.h"
#include <QLabel>

class LIBRUQOLAWIDGETS_TESTS_EXPORT ShowImageGraphicsImageLabel : public QLabel
{
    Q_OBJECT
public:
    explicit ShowImageGraphicsImageLabel(QWidget *parent = nullptr);
    ~ShowImageGraphicsImageLabel() override;

protected:
    void mousePressEvent(QMouseEvent *event) override;
    void mouseMoveEvent(QMouseEvent *event) override;

private:
    QPoint mDragStartPosition;
};
