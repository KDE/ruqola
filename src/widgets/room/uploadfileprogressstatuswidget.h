/*
   SPDX-FileCopyrightText: 2021-2023 Laurent Montel <montel@kde.org>

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

    Q_REQUIRED_RESULT int identifier() const;
    void setIdentifier(int newIdentifier);

Q_SIGNALS:
    void cancelUpload(int identifier);

private:
    int mIdentifier = -1;
    QLabel *const mFileName;
    QProgressBar *const mProgressBar;
    QToolButton *const mCancelToolButton;
};
