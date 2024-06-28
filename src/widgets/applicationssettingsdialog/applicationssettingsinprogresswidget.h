/*
   SPDX-FileCopyrightText: 2024 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#pragma once
#include "libruqolawidgets_private_export.h"
#include <KPixmapSequence>
#include <QWidget>
class QLabel;
class QTimer;
class LIBRUQOLAWIDGETS_TESTS_EXPORT ApplicationsSettingsInProgressWidget : public QWidget
{
    Q_OBJECT
public:
    explicit ApplicationsSettingsInProgressWidget(QWidget *parent = nullptr);
    ~ApplicationsSettingsInProgressWidget() override;

private:
    LIBRUQOLAWIDGETS_NO_EXPORT void slotTimerDone();
    LIBRUQOLAWIDGETS_NO_EXPORT void startAnimation();
    LIBRUQOLAWIDGETS_NO_EXPORT void stopAnimation();
    int mProgressCount = 0;
    KPixmapSequence mProgressPix;
    QLabel *const mLabel;
    QTimer *const mProgressTimer;
};
