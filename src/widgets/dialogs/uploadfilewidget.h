/*
   SPDX-FileCopyrightText: 2020-2024 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#pragma once

#include "libruqolawidgets_private_export.h"
#include <QWidget>
class QLineEdit;
class KUrlRequester;
class QLabel;
class LIBRUQOLAWIDGETS_TESTS_EXPORT UploadFileWidget : public QWidget
{
    Q_OBJECT
public:
    explicit UploadFileWidget(QWidget *parent = nullptr);
    ~UploadFileWidget() override;

    [[nodiscard]] QString description() const;
    [[nodiscard]] QUrl fileUrl() const;
    void setFileUrl(const QUrl &url);
    void setPixmap(const QPixmap &pix);
    void setAuthorizedMediaTypes(const QStringList &mediaTypes);

Q_SIGNALS:
    void updateOkButton(bool enabled);

private:
    QLineEdit *const mDescription;
    KUrlRequester *const mSelectFile;
    QLabel *const mImagePreview;
    QLabel *const mFileLabel;
};
