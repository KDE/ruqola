/*
   SPDX-FileCopyrightText: 2022 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#pragma once

#include "libruqolawidgets_private_export.h"
#include <QWidget>
class RocketChatAccount;
class QToolButton;
class LIBRUQOLAWIDGETS_TESTS_EXPORT ConferenceCallWidget : public QWidget
{
    Q_OBJECT
public:
    struct LIBRUQOLAWIDGETS_TESTS_EXPORT ConferenceCallStart {
        bool useCamera = false;
        bool useMic = false;
    };
    explicit ConferenceCallWidget(RocketChatAccount *account, QWidget *parent = nullptr);
    ~ConferenceCallWidget() override;

    void initialize();

    Q_REQUIRED_RESULT ConferenceCallStart startInfo() const;

private:
    RocketChatAccount *const mRocketChatAccount;
    QToolButton *const mMicroButton;
    QToolButton *const mCameraButton;
};
