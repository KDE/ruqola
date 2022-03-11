/*
   SPDX-FileCopyrightText: 2021-2022 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#pragma once

#include "libruqolawidgets_private_export.h"
#include <QWidget>
class QProgressBar;
class QLabel;
class QToolButton;
class LIBRUQOLAWIDGETS_TESTS_EXPORT UploadFileProgressStatusWidget : public QWidget
{
    Q_OBJECT
public:
    explicit UploadFileProgressStatusWidget(QWidget *parent = nullptr);
    ~UploadFileProgressStatusWidget() override;

    void setValue(int value);
    void setUploadFileName(const QString &str);

Q_SIGNALS:
    void cancelUpload(int identifier = 0); // TODO use identifier

private:
    QLabel *const mFileName;
    QProgressBar *const mProgressBar;
    QToolButton *const mCancelToolButton;
};

