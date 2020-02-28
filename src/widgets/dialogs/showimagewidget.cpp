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

ShowImageWidget::ShowImageWidget(QWidget *parent)
    : QWidget(parent)
{
    auto *mainLayout = new QVBoxLayout(this);
    mainLayout->setObjectName(QStringLiteral("mainLayout"));
    mainLayout->setContentsMargins(0, 0, 0, 0);

    auto *scrollArea = new QScrollArea(this);
    scrollArea->setObjectName(QStringLiteral("scrollArea"));
    scrollArea->setWidgetResizable(true);
    mainLayout->addWidget(scrollArea);

    mLabel = new QLabel(this);
    mLabel->setObjectName(QStringLiteral("mLabel"));
    mLabel->setBackgroundRole(QPalette::Base);

    scrollArea->setWidget(mLabel);

    auto *zoomLayout = new QHBoxLayout;
    zoomLayout->setObjectName(QStringLiteral("zoomLayout"));
    mainLayout->addLayout(zoomLayout);

    QLabel *zoomLabel = new QLabel(i18n("Zoom:"), this);
    zoomLabel->setObjectName(QStringLiteral("zoomLabel"));
    zoomLayout->addWidget(zoomLabel);

    mSlider = new QSlider(this);
    mSlider->setObjectName(QStringLiteral("mSlider"));
    mSlider->setOrientation(Qt::Horizontal);
    zoomLayout->addWidget(mSlider);
    mSlider->setRange(10, 1000);
    mSlider->setValue(100);
    connect(mSlider, &QSlider::valueChanged, this, &ShowImageWidget::slotValueChanged);
}

ShowImageWidget::~ShowImageWidget()
{
}

void ShowImageWidget::slotValueChanged(int value)
{
    if (!mIsAnimatedPixmap) {
        const QPixmap pm = mPixmap.scaled(mPixmap.width()*value/100, mPixmap.height()*value/100, Qt::KeepAspectRatio);
        mLabel->setPixmap(pm);
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
            mSlider->hide();
        }
    }
}

void ShowImageWidget::setImagePath(const QString &imagePath)
{
    QMovie *movie = new QMovie(this);
    movie->setFileName(imagePath);
    mLabel->setMovie(movie);
    movie->start();
}

void ShowImageWidget::setImage(const QPixmap &pix)
{
    mPixmap = pix;
    applyPixmap();
    updateGeometry(); // sizeHint changed
}

QSize ShowImageWidget::sizeHint() const
{
    if (mIsAnimatedPixmap) {
        return QWidget::sizeHint();
    }
    return mPixmap.size().boundedTo(QSize(800, 800));
}

void ShowImageWidget::showEvent(QShowEvent *event)
{
    applyPixmap();
    QWidget::showEvent(event);
}

void ShowImageWidget::resizeEvent(QResizeEvent *event)
{
    applyPixmap();
    QWidget::resizeEvent(event);
}

void ShowImageWidget::applyPixmap()
{
    if (!mIsAnimatedPixmap) {
        mLabel->setPixmap(mPixmap.scaled(mLabel->size(), Qt::KeepAspectRatio));
    }
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
