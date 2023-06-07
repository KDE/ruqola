/*
   SPDX-FileCopyrightText: 2023 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/
#pragma once
#include "libruqolawidgets_private_export.h"
#include <QWidget>
class QPushButton;
class QSlider;
class QToolButton;
class QLabel;
class LIBRUQOLAWIDGETS_TESTS_EXPORT ShowSoundWidget : public QWidget
{
    Q_OBJECT
public:
    explicit ShowSoundWidget(QWidget *parent = nullptr);
    ~ShowSoundWidget() override;

private:
    QPushButton *const mPlayButton;
    QSlider *const mPositionSlider;
    QLabel *const mErrorLabel;
    QToolButton *const mSoundButton;
    QSlider *const mSoundSlider;
    QLabel *const mLabelDuration;
    QLabel *const mLabelPercentSound;
};
