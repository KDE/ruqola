/*
   SPDX-FileCopyrightText: 2020-2024 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
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
    [[nodiscard]] const ShowImageWidget::ImageInfo &imageInfo() const;

    void saveAs();

    void copyImage();
    void copyLocation();

protected:
    void keyPressEvent(QKeyEvent *event) override;

private:
    LIBRUQOLAWIDGETS_NO_EXPORT void updateRanges();
    LIBRUQOLAWIDGETS_NO_EXPORT void slotFileDownloaded(const QString &filePath, const QUrl &cacheImageUrl);
    ImageGraphicsView *const mImageGraphicsView;
    QWidget *const mZoomControls;
    QDoubleSpinBox *const mZoomSpin;
    QSlider *const mSlider;
    RocketChatAccount *const mRocketChatAccount;
};
Q_DECLARE_TYPEINFO(ShowImageWidget::ImageInfo, Q_MOVABLE_TYPE);

class LIBRUQOLAWIDGETS_TESTS_EXPORT ImageGraphicsView : public QGraphicsView
{
    Q_OBJECT
public:
    explicit ImageGraphicsView(RocketChatAccount *account, QWidget *parent = nullptr);
    ~ImageGraphicsView() override;

    void clearContents();

    [[nodiscard]] qreal minimumZoom() const;
    [[nodiscard]] qreal maximumZoom() const;

    [[nodiscard]] qreal zoom() const;
    void setZoom(qreal zoom, QPointF centerPos = {});

    void setImageInfo(const ShowImageWidget::ImageInfo &info);
    [[nodiscard]] const ShowImageWidget::ImageInfo &imageInfo() const;

    void updatePixmap(const QPixmap &pix, const QString &path);
    [[nodiscard]] QPixmap pixmap() const;

    void fitToView();

protected:
    void wheelEvent(QWheelEvent *e) override;

Q_SIGNALS:
    void zoomChanged(qreal zoom);
    void minimumZoomChanged(qreal zoom);
    void maximumZoomChanged(qreal zoom);

private:
    LIBRUQOLAWIDGETS_NO_EXPORT void zoomIn(QPointF centerPos = {});
    LIBRUQOLAWIDGETS_NO_EXPORT void zoomOut(QPointF centerPos = {});
    LIBRUQOLAWIDGETS_NO_EXPORT void updateRanges();
    [[nodiscard]] LIBRUQOLAWIDGETS_NO_EXPORT QSize originalImageSize() const;

    QString mImagePath;

    QSize mOriginalMovieSize;
    QScopedPointer<QMovie> mMovie;
    ShowImageWidget::ImageInfo mImageInfo;

    QGraphicsProxyWidget *mGraphicsProxyWidget = nullptr;
    QGraphicsPixmapItem *mGraphicsPixmapItem = nullptr;
    QLabel *const mAnimatedLabel;
    RocketChatAccount *const mRocketChatAccount;

    qreal mMinimumZoom;
    qreal mMaximumZoom;
    bool mIsUpdatingZoom = false;
};
LIBRUQOLAWIDGETS_EXPORT QDebug operator<<(QDebug d, const ShowImageWidget::ImageInfo &t);
