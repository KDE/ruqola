/*
   Copyright (c) 2020 Laurent Montel <montel@kde.org>

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
#include <QVBoxLayout>
#include <KLocalizedString>
#include <QScrollArea>
#include <QLabel>
#include <QSlider>
#include <QWheelEvent>
#include <QMovie>
#include <QDoubleSpinBox>
#include <QScopedValueRollback>

ShowImageWidget::ShowImageWidget(QWidget *parent)
    : QWidget(parent)
{
    auto mainLayout = new QVBoxLayout(this);
    mainLayout->setObjectName(QStringLiteral("mainLayout"));
    mainLayout->setContentsMargins({});

    auto scrollArea = new QScrollArea(this);
    scrollArea->setObjectName(QStringLiteral("scrollArea"));
    mainLayout->addWidget(scrollArea);

    mLabel = new QLabel(this);
    mLabel->setObjectName(QStringLiteral("mLabel"));
    mLabel->setBackgroundRole(QPalette::Base);
    mLabel->setAlignment(Qt::AlignCenter);

    scrollArea->setWidget(mLabel);

    mZoomControls = new QWidget(this);
    mZoomControls->setObjectName(QStringLiteral("zoomControls"));
    auto zoomLayout = new QHBoxLayout;
    zoomLayout->setObjectName(QStringLiteral("zoomLayout"));
    mZoomControls->setLayout(zoomLayout);
    mainLayout->addWidget(mZoomControls);

    auto zoomLabel = new QLabel(i18n("Zoom:"), this);
    zoomLabel->setObjectName(QStringLiteral("zoomLabel"));
    zoomLayout->addWidget(zoomLabel);

    mZoomSpin = new QDoubleSpinBox(this);
    mZoomSpin->setObjectName(QStringLiteral("mZoomSpin"));
    mZoomSpin->setRange(0.1, 10);
    mZoomSpin->setValue(1);
    mZoomSpin->setDecimals(1);
    mZoomSpin->setSingleStep(0.1);
    zoomLayout->addWidget(mZoomSpin);

    mSlider = new QSlider(this);
    mSlider->setObjectName(QStringLiteral("mSlider"));
    mSlider->setOrientation(Qt::Horizontal);
    zoomLayout->addWidget(mSlider);
    mSlider->setRange(mZoomSpin->minimum() * 100, mZoomSpin->maximum() * 100);
    mSlider->setValue(mZoomSpin->value() * 100);

    connect(mZoomSpin, QOverload<double>::of(&QDoubleSpinBox::valueChanged),
            this, &ShowImageWidget::setZoom);
    connect(mSlider, &QSlider::valueChanged, this, [this](int value) {
        setZoom(static_cast<double>(value) / 100);
    });
}

ShowImageWidget::~ShowImageWidget()
{
}

void ShowImageWidget::setZoom(double scale)
{
    if (!mIsAnimatedPixmap && !mIsUpdatingZoom) {
        QScopedValueRollback<bool> guard(mIsUpdatingZoom, true);
        auto pm = mPixmap.scaled(mPixmap.width() * scale, mPixmap.height() * scale,
                                 Qt::KeepAspectRatio, Qt::SmoothTransformation);
        mLabel->setPixmap(pm);
        mLabel->resize(pm.size());
        mSlider->setValue(static_cast<int>(scale * 100));
        mZoomSpin->setValue(scale);
    }
}

bool ShowImageWidget::isAnimatedPixmap() const
{
    return mIsAnimatedPixmap;
}

void ShowImageWidget::setIsAnimatedPixmap(bool value)
{
    if (mIsAnimatedPixmap != value) {
        mIsAnimatedPixmap = value;
        if (mIsAnimatedPixmap) {
            mZoomControls->hide();
        }
    }
}

void ShowImageWidget::setImagePath(const QString &imagePath)
{
    auto movie = new QMovie(this);
    movie->setFileName(imagePath);
    movie->start();
    const auto size = movie->currentPixmap().size();
    movie->stop();
    mLabel->setMovie(movie);
    mLabel->resize(size);
    movie->start();
}

void ShowImageWidget::setImage(const QPixmap &pix)
{
    mPixmap = pix;
    mPixmap.setDevicePixelRatio(devicePixelRatioF());
    mLabel->setPixmap(mPixmap);
    mLabel->resize(mPixmap.size() / devicePixelRatioF());
    updateGeometry(); // sizeHint changed
}

void ShowImageWidget::wheelEvent(QWheelEvent *e)
{
    if (!mIsAnimatedPixmap) {
        if (e->modifiers() & Qt::ControlModifier) {
            if (e->angleDelta().y() > 0) {
                mSlider->setValue(mSlider->value() - 5);
            } else {
                mSlider->setValue(mSlider->value() + 5);
            }
        }
    }
}
