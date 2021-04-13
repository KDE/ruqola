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

#include "showimagewidget.h"
#include <KLocalizedString>
#include <QDoubleSpinBox>
#include <QGraphicsPixmapItem>
#include <QGraphicsProxyWidget>
#include <QGraphicsScene>
#include <QLabel>
#include <QMovie>
#include <QPushButton>
#include <QScopedValueRollback>
#include <QSlider>
#include <QTimer>
#include <QVBoxLayout>
#include <QWheelEvent>
#include <QtMath>

namespace
{
constexpr qreal defaultMinimumZoomScale = (qreal)0.1;
constexpr qreal defaultMaximumZoomScale = (qreal)10.0;

qreal fitToViewZoomScale(const QSize &imageSize, const QSize &widgetSize)
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

ImageGraphicsView::ImageGraphicsView(QWidget *parent)
    : QGraphicsView(parent)
    , mMinimumZoom(defaultMinimumZoomScale)
    , mMaximumZoom(defaultMaximumZoomScale)
{
    setDragMode(QGraphicsView::ScrollHandDrag);

    auto scene = new QGraphicsScene(this);
    setScene(scene);

    mAnimatedLabel = new QLabel;
    mAnimatedLabel->setObjectName(QStringLiteral("mAnimatedLabel"));
    mAnimatedLabel->setBackgroundRole(QPalette::Base);
    mAnimatedLabel->setAlignment(Qt::AlignCenter);

    mGraphicsProxyWidget = scene->addWidget(mAnimatedLabel);
    mGraphicsProxyWidget->setObjectName(QStringLiteral("mGraphicsProxyWidget"));
    mGraphicsProxyWidget->setFlag(QGraphicsItem::ItemIsMovable, true);

    mGraphicsPixmapItem = scene->addPixmap({});
    mGraphicsPixmapItem->setTransformationMode(Qt::SmoothTransformation);

    updateRanges();
}

ImageGraphicsView::~ImageGraphicsView()
{
}

QPixmap ImageGraphicsView::image() const
{
    return mGraphicsPixmapItem->pixmap();
}

void ImageGraphicsView::setImage(const QPixmap &pixmap)
{
    clearContents();

    mGraphicsPixmapItem->setPixmap(pixmap);
    QTimer::singleShot(0, this, [=] {
        updateRanges();

        fitToView();
    });
}

QString ImageGraphicsView::imagePath() const
{
    return mMovie ? mMovie->fileName() : QString();
}

void ImageGraphicsView::setImagePath(const QString &imagePath)
{
    clearContents();

    mMovie.reset(new QMovie(this));
    mMovie->setFileName(imagePath);
    mMovie->start();
    mMovie->stop();
    mAnimatedLabel->setMovie(mMovie.data());

    QTimer::singleShot(0, this, [=] {
        mOriginalMovieSize = mMovie->currentPixmap().size();
        updateRanges();

        fitToView();
        mMovie->start();
    });
}

void ImageGraphicsView::zoomIn(const QPointF &centerPos)
{
    setZoom(zoom() * 1.1, centerPos);
}

void ImageGraphicsView::zoomOut(const QPointF &centerPos)
{
    setZoom(zoom() * 0.9, centerPos);
}

void ImageGraphicsView::clearContents()
{
    mOriginalMovieSize = {};
    mAnimatedLabel->setMovie(nullptr);
    mMovie.reset();

    mGraphicsPixmapItem->setPixmap({});
}

qreal ImageGraphicsView::minimumZoom() const
{
    return mMinimumZoom;
}

qreal ImageGraphicsView::maximumZoom() const
{
    return mMaximumZoom;
}

void ImageGraphicsView::updateRanges()
{
    const auto newMinimumZoom = fitToViewZoomScale(originalImageSize(), size());
    if (!qFuzzyCompare(mMinimumZoom, newMinimumZoom)) {
        mMinimumZoom = fitToViewZoomScale(originalImageSize(), size());
        Q_EMIT minimumZoomChanged(mMinimumZoom);
    }
    // note: mMaximumZoom is constant for now
}

void ImageGraphicsView::wheelEvent(QWheelEvent *e)
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

QSize ImageGraphicsView::originalImageSize() const
{
    if (mOriginalMovieSize.isValid()) {
        return mOriginalMovieSize;
    }

    return mGraphicsPixmapItem->pixmap().size();
}

qreal ImageGraphicsView::zoom() const
{
    return transform().m11();
}

void ImageGraphicsView::setZoom(qreal zoom, const QPointF &centerPos)
{
    // clamp value
    zoom = qBound(minimumZoom(), zoom, maximumZoom());

    if (qFuzzyCompare(this->zoom(), zoom))
        return;

    if (mIsUpdatingZoom)
        return;

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

void ImageGraphicsView::fitToView()
{
    setZoom(fitToViewZoomScale(originalImageSize(), size()));
    centerOn(mGraphicsPixmapItem);
}

ShowImageWidget::ShowImageWidget(QWidget *parent)
    : QWidget(parent)
    , mZoomControls(new QWidget(this))
    , mZoomSpin(new QDoubleSpinBox(this))
    , mSlider(new QSlider(this))
{
    auto mainLayout = new QVBoxLayout(this);
    mainLayout->setObjectName(QStringLiteral("mainLayout"));
    mainLayout->setContentsMargins({});

    mImageGraphicsView = new ImageGraphicsView(this);
    mImageGraphicsView->setObjectName(QStringLiteral("mImageGraphicsView"));
    mainLayout->addWidget(mImageGraphicsView);
    connect(mImageGraphicsView, &ImageGraphicsView::zoomChanged, this, [this](qreal zoom) {
        mSlider->setValue(static_cast<int>(zoom * 100));
        mZoomSpin->setValue(zoom);
    });
    connect(mImageGraphicsView, &ImageGraphicsView::minimumZoomChanged, this, &ShowImageWidget::updateRanges);
    connect(mImageGraphicsView, &ImageGraphicsView::maximumZoomChanged, this, &ShowImageWidget::updateRanges);

    mZoomControls->setObjectName(QStringLiteral("zoomControls"));
    auto zoomLayout = new QHBoxLayout;
    zoomLayout->setObjectName(QStringLiteral("zoomLayout"));
    mZoomControls->setLayout(zoomLayout);
    mainLayout->addWidget(mZoomControls);

    auto mLabel = new QLabel(i18n("Zoom:"), this);
    mLabel->setObjectName(QStringLiteral("zoomLabel"));
    zoomLayout->addWidget(mLabel);

    mZoomSpin->setObjectName(QStringLiteral("mZoomSpin"));

    mZoomSpin->setValue(1);
    mZoomSpin->setDecimals(1);
    mZoomSpin->setSingleStep(0.1);
    zoomLayout->addWidget(mZoomSpin);

    mSlider->setObjectName(QStringLiteral("mSlider"));
    mSlider->setOrientation(Qt::Horizontal);
    zoomLayout->addWidget(mSlider);
    mSlider->setValue(mZoomSpin->value() * 100.0);

    auto resetButton = new QPushButton(this);
    resetButton->setObjectName(QStringLiteral("resetButton"));
    resetButton->setText(i18n("100%"));
    zoomLayout->addWidget(resetButton);
    connect(resetButton, &QPushButton::clicked, this, [=] {
        mImageGraphicsView->setZoom(1.0);
    });

    auto fitToViewButton = new QPushButton(this);
    fitToViewButton->setObjectName(QStringLiteral("fitToViewButton"));
    fitToViewButton->setText(i18n("Fit to View"));
    zoomLayout->addWidget(fitToViewButton);
    connect(fitToViewButton, &QPushButton::clicked, mImageGraphicsView, &ImageGraphicsView::fitToView);

    connect(mZoomSpin, QOverload<double>::of(&QDoubleSpinBox::valueChanged), this, [this](double value) {
        mImageGraphicsView->setZoom(static_cast<qreal>(value));
    });
    connect(mSlider, &QSlider::valueChanged, this, [this](int value) {
        mImageGraphicsView->setZoom(static_cast<qreal>(value) / 100);
    });

    updateRanges();
}

ShowImageWidget::~ShowImageWidget()
{
}

bool ShowImageWidget::isAnimatedPixmap() const
{
    return mIsAnimatedPixmap;
}

void ShowImageWidget::setIsAnimatedPixmap(bool value)
{
    if (mIsAnimatedPixmap != value) {
        mIsAnimatedPixmap = value;
    }
}

void ShowImageWidget::updateRanges()
{
    const auto min = mImageGraphicsView->minimumZoom();
    const auto max = mImageGraphicsView->maximumZoom();
    mZoomSpin->setRange(min, max);
    mSlider->setRange(min * 100.0, max * 100.0);
}

void ShowImageWidget::setImagePath(const QString &imagePath)
{
    mImageGraphicsView->setImagePath(imagePath);
}

void ShowImageWidget::setImage(const QPixmap &pixmap)
{
    mImageGraphicsView->setImage(pixmap);
}
