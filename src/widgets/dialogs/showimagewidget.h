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
class ImageGraphicsView;
class RocketChatAccount;
class LIBRUQOLAWIDGETS_TESTS_EXPORT ShowImageWidget : public QWidget
{
    Q_OBJECT
public:
    struct ImageInfo {
        QString bigImagePath;
        QString previewImagePath;
        QPixmap pixmap;
        bool isAnimatedImage = false;
        bool needToDownloadBigImage = false;
    };

    explicit ShowImageWidget(RocketChatAccount *account, QWidget *parent = nullptr);
    ~ShowImageWidget() override;

    void setImageInfo(const ImageInfo &info);
    Q_REQUIRED_RESULT const ShowImageWidget::ImageInfo &imageInfo() const;

    void saveAs();

private:
    void updateRanges();
    void slotFileDownloaded(const QString &filePath, const QUrl &cacheImageUrl);
    ImageGraphicsView *const mImageGraphicsView;
    QWidget *const mZoomControls;
    QDoubleSpinBox *const mZoomSpin;
    QSlider *const mSlider;
    RocketChatAccount *const mRocketChatAccount;
};

class LIBRUQOLAWIDGETS_TESTS_EXPORT ImageGraphicsView : public QGraphicsView
{
    Q_OBJECT
    Q_PROPERTY(qreal zoom READ zoom WRITE setZoom NOTIFY zoomChanged)
    Q_PROPERTY(qreal minimumZoom READ minimumZoom NOTIFY minimumZoomChanged)
    Q_PROPERTY(qreal maximumZoom READ maximumZoom NOTIFY maximumZoomChanged)

public:
    explicit ImageGraphicsView(QWidget *parent = nullptr);
    ~ImageGraphicsView() override;

    void clearContents();

    Q_REQUIRED_RESULT qreal minimumZoom() const;
    Q_REQUIRED_RESULT qreal maximumZoom() const;

    Q_REQUIRED_RESULT qreal zoom() const;
    void setZoom(qreal zoom, QPointF centerPos = {});

    void setImageInfo(const ShowImageWidget::ImageInfo &info);
    Q_REQUIRED_RESULT const ShowImageWidget::ImageInfo &imageInfo() const;

public Q_SLOTS:
    void zoomIn(QPointF centerPos = {});
    void zoomOut(QPointF centerPos = {});
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

    QString mImagePath;

    QSize mOriginalMovieSize;
    QScopedPointer<QMovie> mMovie;
    ShowImageWidget::ImageInfo mImageInfo;

    QGraphicsProxyWidget *mGraphicsProxyWidget = nullptr;
    QGraphicsPixmapItem *mGraphicsPixmapItem = nullptr;
    QLabel *const mAnimatedLabel;

    qreal mMinimumZoom;
    qreal mMaximumZoom;
    bool mIsUpdatingZoom = false;
};
LIBRUQOLAWIDGETS_EXPORT QDebug operator<<(QDebug d, const ShowImageWidget::ImageInfo &t);
