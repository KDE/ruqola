/*
   SPDX-FileCopyrightText: 2020-2026 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#pragma once

#include <QDialog>
#include <QUrl>

#include "libruqolawidgets_export.h"
class UploadFileWidget;
class QPushButton;
class LIBRUQOLAWIDGETS_EXPORT UploadFileDialog : public QDialog
{
    Q_OBJECT
public:
    struct UploadFileInfo {
        QString description;
        QString fileName;
        QUrl fileUrl;
        bool deleteTemporaryFile = false;
        [[nodiscard]] bool isValid() const
        {
            return fileUrl.isValid() && fileUrl.isLocalFile();
        }
    };

    explicit UploadFileDialog(QWidget *parent = nullptr);
    ~UploadFileDialog() override;

    void setFileUrl(const QUrl &url);

    [[nodiscard]] UploadFileInfo fileInfo() const;

private:
    UploadFileWidget *const mUploadFileWidget;
    QPushButton *mOkButton = nullptr;
};
