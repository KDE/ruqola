/*
   SPDX-FileCopyrightText: 2020-2024 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#pragma once

#include <QDialog>
#include <QUrl>

#include "libruqolawidgets_private_export.h"
class UploadFileWidget;
class QPushButton;
class LIBRUQOLAWIDGETS_TESTS_EXPORT UploadFileDialog : public QDialog
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

    void setAuthorizedMediaTypes(const QStringList &mediaTypes);

    void setPixmap(const QPixmap &pix);

private:
    LIBRUQOLAWIDGETS_NO_EXPORT void readConfig();
    LIBRUQOLAWIDGETS_NO_EXPORT void writeConfig();
    UploadFileWidget *const mUploadFileWidget;
    QPushButton *mOkButton = nullptr;
};
