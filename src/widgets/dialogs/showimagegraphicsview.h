/*
   SPDX-FileCopyrightText: 2020-2025 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#pragma once

#include <QGraphicsView>
class QWidget;

#include "libruqolawidgets_private_export.h"
#include "showimagewidget.h"
class RocketChatAccount;
class ShowImageGraphicsImageLabel;
class ShowImageGraphicsPixmapItem;

class LIBRUQOLAWIDGETS_TESTS_EXPORT ShowImageGraphicsView : public QGraphicsView
{
    Q_OBJECT
public:
    explicit ShowImageGraphicsView(RocketChatAccount *account, QWidget *parent = nullptr);
    ~ShowImageGraphicsView() override;

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
    ShowImageGraphicsPixmapItem *const mGraphicsPixmapItem;
    ShowImageGraphicsImageLabel *const mAnimatedLabel;
    RocketChatAccount *const mRocketChatAccount;

    qreal mMinimumZoom;
    qreal mMaximumZoom;
    bool mIsUpdatingZoom = false;
};
