/*
   SPDX-FileCopyrightText: 2025-2026 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#pragma once
#include "libruqolawidgets_private_export.h"
#include <QWidget>
class QToolButton;
class LIBRUQOLAWIDGETS_TESTS_EXPORT ShowImagePrevNextImageWidget : public QWidget
{
    Q_OBJECT
public:
    explicit ShowImagePrevNextImageWidget(QWidget *parent = nullptr);
    ~ShowImagePrevNextImageWidget() override;

    void setUpdateButtons(bool hasPrevious, bool hasNext);
Q_SIGNALS:
    void showNextImage();
    void showPreviousImage();

private:
    QToolButton *const mNextButton;
    QToolButton *const mPreviousButton;
};
