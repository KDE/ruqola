/*
   SPDX-FileCopyrightText: 2023 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#pragma once
#include "libruqolawidgets_private_export.h"
#include <QWidget>
class QToolButton;
class QLabel;
class LIBRUQOLAWIDGETS_TESTS_EXPORT CreateSoundMessageWidget : public QWidget
{
    Q_OBJECT
public:
    explicit CreateSoundMessageWidget(QWidget *parent = nullptr);
    ~CreateSoundMessageWidget() override;

private:
    void stop();
    void record();
    void pause();
    QToolButton *const mRecordButton;
    QToolButton *const mPauseButton;
    QToolButton *const mStopButton;
    QLabel *const mLabelDuration;
};
