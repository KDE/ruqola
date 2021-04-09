/*
   Copyright (c) 2020-2021 Laurent Montel <montel@kde.org>

   This library is free software; you can redistribute it and/or modify
   it under the terms of the GNU Library General Public License as published
   by the Free Software Foundation; either version 2 of the License or
   ( at your option ) version 3 or, at the discretion of KDE e.V.
   ( which shall act as a proxy as in section 14 of the GPLv3 ), any later version.

   This library is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
   Library General Public License for more details.

   You should have received a copy of the GNU Library General Public License
   along with this library; see the file COPYING.LIB.  If not, write to
   the Free Software Foundation, Inc., 51 Franklin Street, Fifth Floor,
   Boston, MA 02110-1301, USA.
*/

#pragma once

#include <QGraphicsView>
#include <QWidget>

#include "libruqolawidgets_private_export.h"

class QLabel;
class QSlider;
class QDoubleSpinBox;
class QMovie;

class LIBRUQOLAWIDGETS_TESTS_EXPORT ImageGraphicsView : public QGraphicsView
{
    Q_OBJECT
    Q_PROPERTY(qreal zoom READ zoom WRITE setZoom NOTIFY zoomChanged)
    Q_PROPERTY(qreal minimumZoom READ minimumZoom NOTIFY minimumZoomChanged)
    Q_PROPERTY(qreal maximumZoom READ maximumZoom NOTIFY maximumZoomChanged)

public:
    explicit ImageGraphicsView(QWidget *parent = nullptr);
    ~ImageGraphicsView() override;

    Q_REQUIRED_RESULT QPixmap image() const;
    void setImage(const QPixmap &pixmap);
    Q_REQUIRED_RESULT QString imagePath() const;
    void setImagePath(const QString &imagePath);
    void clearContents();

    Q_REQUIRED_RESULT qreal minimumZoom() const;
    Q_REQUIRED_RESULT qreal maximumZoom() const;

    Q_REQUIRED_RESULT qreal zoom() const;
    void setZoom(qreal zoom, const QPointF &centerPos = {});

public Q_SLOTS:
    void zoomIn(const QPointF &centerPos = {});
    void zoomOut(const QPointF &centerPos = {});
    void fitToView();

protected:
    void wheelEvent(QWheelEvent *e) override;

Q_SIGNALS:
    void zoomChanged(qreal zoom);
    void minimumZoomChanged(qreal zoom);
    void maximumZoomChanged(qreal zoom);

private:
    void updateRanges();
    QSize originalImageSize() const;

    QLabel *mAnimatedLabel = nullptr;
    QSize mOriginalMovieSize;
    QScopedPointer<QMovie> mMovie;

    QGraphicsProxyWidget *mGraphicsProxyWidget = nullptr;
    QGraphicsPixmapItem *mGraphicsPixmapItem = nullptr;

    qreal mMinimumZoom;
    qreal mMaximumZoom;
    bool mIsUpdatingZoom = false;
};

class LIBRUQOLAWIDGETS_TESTS_EXPORT ShowImageWidget : public QWidget
{
    Q_OBJECT
public:
    explicit ShowImageWidget(QWidget *parent = nullptr);
    ~ShowImageWidget() override;

    void setImage(const QPixmap &pixmap);
    void setImagePath(const QString &imagePath);

    Q_REQUIRED_RESULT bool isAnimatedPixmap() const;
    void setIsAnimatedPixmap(bool value);

private Q_SLOTS:
    void updateRanges();

private:
    ImageGraphicsView *mImageGraphicsView = nullptr;
    QWidget *const mZoomControls;
    QDoubleSpinBox *const mZoomSpin;
    QSlider *const mSlider;
    bool mIsAnimatedPixmap = false;
};
