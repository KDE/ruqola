/*
   SPDX-FileCopyrightText: 2020-2025 Laurent Montel <montel@kde.org>

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
class ShowImageGraphicsView;
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
    ShowImageGraphicsView *const mImageGraphicsView;
    QWidget *const mZoomControls;
    QDoubleSpinBox *const mZoomSpin;
    QSlider *const mSlider;
    RocketChatAccount *const mRocketChatAccount;
};
Q_DECLARE_TYPEINFO(ShowImageWidget::ImageInfo, Q_RELOCATABLE_TYPE);
LIBRUQOLAWIDGETS_EXPORT QDebug operator<<(QDebug d, const ShowImageWidget::ImageInfo &t);
