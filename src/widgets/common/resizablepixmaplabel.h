/*
   SPDX-FileCopyrightText: 2025-2026 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/
#pragma once
#include "libruqolawidgets_private_export.h"
#include <QLabel>
#include <QPixmap>
class LIBRUQOLAWIDGETS_TESTS_EXPORT ResizablePixmapLabel : public QLabel
{
    Q_OBJECT
public:
    explicit ResizablePixmapLabel(QWidget *parent = nullptr);
    ~ResizablePixmapLabel() override;
    void setCurrentPixmap(const QPixmap &pix);

protected:
    void resizeEvent(QResizeEvent *event) override;

private:
    QPixmap mPix;
};
