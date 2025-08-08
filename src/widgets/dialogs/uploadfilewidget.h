/*
   SPDX-FileCopyrightText: 2020-2025 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#pragma once

#include "libruqolawidgets_private_export.h"
#include <QUrl>
#include <QWidget>
class QLineEdit;
class QLabel;
class LIBRUQOLAWIDGETS_TESTS_EXPORT UploadFileWidget : public QWidget
{
    Q_OBJECT
public:
    explicit UploadFileWidget(QWidget *parent = nullptr);
    ~UploadFileWidget() override;

    [[nodiscard]] QString description() const;
    [[nodiscard]] QUrl fileUrl() const;
    [[nodiscard]] QString fileName() const;
    void setFileUrl(const QUrl &url);
    void setPixmap(const QPixmap &pix);

Q_SIGNALS:
    void uploadImage();

protected:
    void resizeEvent(QResizeEvent *event) override;

private:
    QLineEdit *const mFileName;
    QLineEdit *const mDescription;
    QLabel *const mImagePreview;
    QLabel *const mFileNameInfo;
    QLabel *const mMimeTypeLabel;
    QUrl mUrl;
    QPixmap mPix;
};
