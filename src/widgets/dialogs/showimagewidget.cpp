/*
   SPDX-FileCopyrightText: 2020-2025 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "showimagewidget.h"
#include "common/delegateutil.h"
#include "rocketchataccount.h"
#include "showimagegraphicsview.h"

#include "ruqolawidgets_showimage_debug.h"
#include <KLocalizedString>
#include <QApplication>
#include <QClipboard>
#include <QDoubleSpinBox>

#include <QKeyEvent>
#include <QLabel>
#include <QMimeData>
#include <QPushButton>
#include <QSlider>
#include <QVBoxLayout>

ShowImageWidget::ShowImageWidget(RocketChatAccount *account, QWidget *parent)
    : QWidget(parent)
    , mImageGraphicsView(new ShowImageGraphicsView(account, this))
    , mZoomControls(new QWidget(this))
    , mZoomSpin(new QDoubleSpinBox(this))
    , mSlider(new QSlider(this))
    , mRocketChatAccount(account)
{
    auto mainLayout = new QVBoxLayout(this);
    mainLayout->setObjectName(QStringLiteral("mainLayout"));
    mainLayout->setContentsMargins({});

    mImageGraphicsView->setObjectName(QStringLiteral("mImageGraphicsView"));
    mainLayout->addWidget(mImageGraphicsView);
    connect(mImageGraphicsView, &ShowImageGraphicsView::zoomChanged, this, [this](qreal zoom) {
        mSlider->setValue(static_cast<int>(zoom * 100));
        mZoomSpin->setValue(zoom);
    });
    connect(mImageGraphicsView, &ShowImageGraphicsView::minimumZoomChanged, this, &ShowImageWidget::updateRanges);
    connect(mImageGraphicsView, &ShowImageGraphicsView::maximumZoomChanged, this, &ShowImageWidget::updateRanges);

    mZoomControls->setObjectName(QStringLiteral("zoomControls"));
    auto zoomLayout = new QHBoxLayout;
    zoomLayout->setObjectName(QStringLiteral("zoomLayout"));
    mZoomControls->setLayout(zoomLayout);
    mainLayout->addWidget(mZoomControls);

    auto mLabel = new QLabel(i18nc("@label:textbox", "Zoom:"), this);
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

    auto resetButton = new QPushButton(i18nc("@action:button", "100%"), this);
    resetButton->setObjectName(QStringLiteral("resetButton"));
    zoomLayout->addWidget(resetButton);
    connect(resetButton, &QPushButton::clicked, this, [this] {
        mImageGraphicsView->setZoom(1.0);
    });

    auto fitToViewButton = new QPushButton(i18nc("@action:button", "Fit to View"), this);
    fitToViewButton->setObjectName(QStringLiteral("fitToViewButton"));
    zoomLayout->addWidget(fitToViewButton);
    connect(fitToViewButton, &QPushButton::clicked, mImageGraphicsView, &ShowImageGraphicsView::fitToView);

    connect(mZoomSpin, &QDoubleSpinBox::valueChanged, this, [this](double value) {
        mImageGraphicsView->setZoom(static_cast<qreal>(value));
    });
    connect(mSlider, &QSlider::valueChanged, this, [this](int value) {
        mImageGraphicsView->setZoom(static_cast<qreal>(value) / 100);
    });

    if (mRocketChatAccount) {
        connect(mRocketChatAccount, &RocketChatAccount::fileDownloaded, this, &ShowImageWidget::slotFileDownloaded);
    }
    updateRanges();
}

ShowImageWidget::~ShowImageWidget() = default;

void ShowImageWidget::keyPressEvent(QKeyEvent *event)
{
    const bool isControlClicked = event->modifiers() & Qt::ControlModifier;
    if (isControlClicked) {
        if (event->key() == Qt::Key_Plus) {
            mSlider->setValue(mSlider->value() + mSlider->singleStep());
        } else if (event->key() == Qt::Key_Minus) {
            mSlider->setValue(mSlider->value() - mSlider->singleStep());
        }
    } else {
        QWidget::keyPressEvent(event);
    }
}

void ShowImageWidget::slotFileDownloaded(const QString &filePath, const QUrl &cacheImageUrl)
{
    qCDebug(RUQOLAWIDGETS_SHOWIMAGE_LOG) << "File Downloaded : " << filePath << " cacheImageUrl " << cacheImageUrl;
    const ImageInfo &info = imageInfo();
    qCDebug(RUQOLAWIDGETS_SHOWIMAGE_LOG) << "info.bigImagePath  " << info.bigImagePath;
    if (filePath == QUrl(info.bigImagePath).toString()) {
        qCDebug(RUQOLAWIDGETS_SHOWIMAGE_LOG) << "Update image  " << info << "filePath" << filePath << "cacheImageUrl " << cacheImageUrl;
        const QString cacheImageUrlPath{cacheImageUrl.toLocalFile()};
        const QPixmap pixmap(cacheImageUrlPath);
        mImageGraphicsView->updatePixmap(pixmap, cacheImageUrlPath);
    }
}

void ShowImageWidget::updateRanges()
{
    const auto min = mImageGraphicsView->minimumZoom();
    const auto max = mImageGraphicsView->maximumZoom();
    mZoomSpin->setRange(min, max);
    mSlider->setRange(min * 100.0, max * 100.0);
}

void ShowImageWidget::setImageInfo(const ShowImageWidget::ImageInfo &info)
{
    mImageGraphicsView->setImageInfo(info);
}

const ShowImageWidget::ImageInfo &ShowImageWidget::imageInfo() const
{
    return mImageGraphicsView->imageInfo();
}

void ShowImageWidget::saveAs()
{
    DelegateUtil::saveFile(this,
                           mRocketChatAccount->attachmentUrlFromLocalCache(mImageGraphicsView->imageInfo().bigImagePath).toLocalFile(),
                           i18n("Save Image"));
}

void ShowImageWidget::copyImage()
{
    auto data = new QMimeData();
    data->setImageData(mImageGraphicsView->pixmap().toImage());
    data->setData(QStringLiteral("x-kde-force-image-copy"), QByteArray());
    QApplication::clipboard()->setMimeData(data, QClipboard::Clipboard);
}

void ShowImageWidget::copyLocation()
{
    const QString imagePath = mRocketChatAccount->attachmentUrlFromLocalCache(mImageGraphicsView->imageInfo().bigImagePath).toLocalFile();
    QApplication::clipboard()->setText(imagePath);
}

QDebug operator<<(QDebug d, const ShowImageWidget::ImageInfo &t)
{
    d.space() << "bigImagePath : " << t.bigImagePath;
    d.space() << "previewImagePath : " << t.previewImagePath;
    d.space() << "isAnimatedImage : " << t.isAnimatedImage;
    d.space() << "pixmap is null ? " << t.pixmap.isNull();
    d.space() << "needToDownloadBigImage ? " << t.needToDownloadBigImage;
    return d;
}

#include "moc_showimagewidget.cpp"
