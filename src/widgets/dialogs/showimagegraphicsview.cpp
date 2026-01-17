/*
   SPDX-FileCopyrightText: 2020-2026 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "showimagegraphicsview.h"

#include "rocketchataccount.h"
#include "ruqolawidgets_showimage_debug.h"
#include "showimagegraphicsimagelabel.h"
#include "showimagegraphicspixmapitem.h"

#include <QGraphicsProxyWidget>
#include <QGraphicsScene>
#include <QMovie>
#include <QScopedValueRollback>
#include <QTimer>
using namespace Qt::Literals::StringLiterals;
namespace
{
constexpr qreal defaultMinimumZoomScale = (qreal)0.1;
constexpr qreal defaultMaximumZoomScale = (qreal)10.0;

qreal fitToViewZoomScale(QSize imageSize, QSize widgetSize)
{
    if (imageSize.width() > widgetSize.width() || imageSize.height() > widgetSize.height()) {
        // Make sure it fits, we care only about the first two decimal points, so round to the smaller value
        const qreal hZoom = (qreal)widgetSize.width() / imageSize.width();
        const qreal vZoom = (qreal)widgetSize.height() / imageSize.height();
        return std::max((int)(std::min(hZoom, vZoom) * 100) / 100.0, defaultMinimumZoomScale);
    }
    return 1.0;
}
}

ShowImageGraphicsView::ShowImageGraphicsView(RocketChatAccount *account, QWidget *parent)
    : QGraphicsView(parent)
    , mGraphicsPixmapItem(new ShowImageGraphicsPixmapItem)
    , mAnimatedLabel(new ShowImageGraphicsImageLabel)
    , mRocketChatAccount(account)
    , mMinimumZoom(defaultMinimumZoomScale)
    , mMaximumZoom(defaultMaximumZoomScale)
{
    setDragMode(QGraphicsView::ScrollHandDrag);

    auto scene = new QGraphicsScene(this);
    setScene(scene);

    mAnimatedLabel->setObjectName(u"mAnimatedLabel"_s);
    mAnimatedLabel->setBackgroundRole(QPalette::Base);
    mAnimatedLabel->setAlignment(Qt::AlignCenter);

    mGraphicsProxyWidget = scene->addWidget(mAnimatedLabel);
    mGraphicsProxyWidget->setObjectName(u"mGraphicsProxyWidget"_s);
    mGraphicsProxyWidget->setFlag(QGraphicsItem::ItemIsMovable, true);

    scene->addItem(mGraphicsPixmapItem);
    mGraphicsPixmapItem->setTransformationMode(Qt::SmoothTransformation);

    updateRanges();
}

ShowImageGraphicsView::~ShowImageGraphicsView() = default;

void ShowImageGraphicsView::updatePixmap(const QPixmap &pix, const QString &path)
{
    clearContents();
    if (!mImageInfo.isAnimatedImage) {
        mGraphicsPixmapItem->setImagePath(path);
        mGraphicsPixmapItem->setPixmap(pix);
        QTimer::singleShot(0, this, [this] {
            updateRanges();

            fitToView();
        });
    } else {
        mMovie.reset(new QMovie(this));
        mMovie->setFileName(path);
        mMovie->start();
        mMovie->stop();
        mAnimatedLabel->setImagePath(path);
        mAnimatedLabel->setMovie(mMovie.data());

        QTimer::singleShot(0, this, [this] {
            mOriginalMovieSize = mMovie->currentPixmap().size();
            updateRanges();

            fitToView();
            mMovie->start();
        });
    }
}

void ShowImageGraphicsView::setImageInfo(const ShowImageWidget::ImageInfo &info)
{
    mImageInfo = info;
    qCDebug(RUQOLAWIDGETS_SHOWIMAGE_LOG) << "ShowImageWidget::ImageInfo  " << info;
    if (info.needToDownloadBigImage) {
        qCDebug(RUQOLAWIDGETS_SHOWIMAGE_LOG) << " Download big image " << info.needToDownloadBigImage << " use same image";
        // We just need to download image not get url as it will be empty as we need to download it.
        if (mRocketChatAccount) {
            (void)mRocketChatAccount->attachmentUrlFromLocalCache(info.bigImagePath);
        }
        updatePixmap(mImageInfo.pixmap, mImageInfo.bigImagePath);
    } else {
        // Use big image.
        if (mRocketChatAccount) {
            qCDebug(RUQOLAWIDGETS_SHOWIMAGE_LOG) << " Big image already downloaded " << info.needToDownloadBigImage;
            const QString pixBigImagePath{mRocketChatAccount->attachmentUrlFromLocalCache(mImageInfo.bigImagePath).toLocalFile()};
            const QPixmap pix(pixBigImagePath);
            updatePixmap(pix, pixBigImagePath);
        }
    }
}

void ShowImageGraphicsView::zoomIn(QPointF centerPos)
{
    setZoom(zoom() * 1.1, centerPos);
}

void ShowImageGraphicsView::zoomOut(QPointF centerPos)
{
    setZoom(zoom() * 0.9, centerPos);
}

void ShowImageGraphicsView::clearContents()
{
    mOriginalMovieSize = {};
    mAnimatedLabel->setMovie(nullptr);
    mMovie.reset();

    mGraphicsPixmapItem->setPixmap({});
}

QPixmap ShowImageGraphicsView::pixmap() const
{
    return mGraphicsPixmapItem->pixmap();
}

qreal ShowImageGraphicsView::minimumZoom() const
{
    return mMinimumZoom;
}

qreal ShowImageGraphicsView::maximumZoom() const
{
    return mMaximumZoom;
}

void ShowImageGraphicsView::updateRanges()
{
    const auto newMinimumZoom = fitToViewZoomScale(originalImageSize(), size());
    if (!qFuzzyCompare(mMinimumZoom, newMinimumZoom)) {
        mMinimumZoom = fitToViewZoomScale(originalImageSize(), size());
        Q_EMIT minimumZoomChanged(mMinimumZoom);
    }
    // note: mMaximumZoom is constant for now
}

void ShowImageGraphicsView::wheelEvent(QWheelEvent *e)
{
    if (e->modifiers() == Qt::ControlModifier) {
        const int y = e->angleDelta().y();
        if (y < 0) {
            zoomOut(e->position());
        } else if (y > 0) {
            zoomIn(e->position());
        } // else: y == 0 => horizontal scroll => do not handle
    } else {
        QGraphicsView::wheelEvent(e);
    }
}

QSize ShowImageGraphicsView::originalImageSize() const
{
    if (mOriginalMovieSize.isValid()) {
        return mOriginalMovieSize;
    }

    return mGraphicsPixmapItem->pixmap().size();
}

const ShowImageWidget::ImageInfo &ShowImageGraphicsView::imageInfo() const
{
    return mImageInfo;
}

qreal ShowImageGraphicsView::zoom() const
{
    return transform().m11();
}

void ShowImageGraphicsView::setZoom(qreal zoom, QPointF centerPos)
{
    // clamp value
    zoom = qBound(minimumZoom(), zoom, maximumZoom());

    if (qFuzzyCompare(this->zoom(), zoom)) {
        return;
    }

    if (mIsUpdatingZoom) {
        return;
    }

    QScopedValueRollback<bool> guard(mIsUpdatingZoom, true);

    QPointF targetScenePos;
    if (!centerPos.isNull()) {
        targetScenePos = mapToScene(centerPos.toPoint());
    } else {
        targetScenePos = sceneRect().center();
    }

    ViewportAnchor oldAnchor = this->transformationAnchor();
    setTransformationAnchor(QGraphicsView::NoAnchor);

    QTransform matrix;
    matrix.translate(targetScenePos.x(), targetScenePos.y()).scale(zoom, zoom).translate(-targetScenePos.x(), -targetScenePos.y());
    setTransform(matrix);

    setTransformationAnchor(oldAnchor);
    Q_EMIT zoomChanged(zoom);
}

void ShowImageGraphicsView::fitToView()
{
    setZoom(fitToViewZoomScale(originalImageSize(), size()));
    centerOn(mGraphicsPixmapItem);
}

#include "moc_showimagegraphicsview.cpp"
