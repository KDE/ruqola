/*
   SPDX-FileCopyrightText: 2025 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/
#pragma once

#include <QLabel>
#include <QPixmap>
class ResizablePixmapLabel : public QLabel
{
    Q_OBJECT
public:
    explicit ResizablePixmapLabel(QWidget *parent = nullptr);
    ~ResizablePixmapLabel() override;
    void setPixmap(const QPixmap &pix);

protected:
    void resizeEvent(QResizeEvent *event) override;

private:
    QPixmap mPix;
};
