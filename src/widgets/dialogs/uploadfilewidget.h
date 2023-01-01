/*
   SPDX-FileCopyrightText: 2020-2023 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#pragma once

#include "libruqolawidgets_private_export.h"
#include <QWidget>
class QLineEdit;
class KUrlRequester;
class LIBRUQOLAWIDGETS_TESTS_EXPORT UploadFileWidget : public QWidget
{
    Q_OBJECT
public:
    explicit UploadFileWidget(QWidget *parent = nullptr);
    ~UploadFileWidget() override;

    Q_REQUIRED_RESULT QString description() const;
    Q_REQUIRED_RESULT QUrl fileUrl() const;
    void setFileUrl(const QUrl &url);

Q_SIGNALS:
    void updateOkButton(bool enabled);

private:
    QLineEdit *const mDescription;
    KUrlRequester *const mSelectFile;
};
