/*
   SPDX-FileCopyrightText: 2020-2022 Laurent Montel <montel@kde.org>

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
        QString message;
        QUrl fileUrl;
        Q_REQUIRED_RESULT bool isValid() const
        {
            return fileUrl.isValid() && fileUrl.isLocalFile();
        }
    };

    explicit UploadFileDialog(QWidget *parent = nullptr);
    ~UploadFileDialog() override;

    void setFileUrl(const QUrl &url);

    Q_REQUIRED_RESULT UploadFileInfo fileInfo() const;

private:
    void readConfig();
    void writeConfig();
    UploadFileWidget *const mUploadFileWidget;
    QPushButton *mOkButton = nullptr;
};

