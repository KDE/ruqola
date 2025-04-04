/*
   SPDX-FileCopyrightText: 2022-2025 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#pragma once

#include "libruqolawidgets_private_export.h"
#include <QWidget>
class QToolButton;
class LIBRUQOLAWIDGETS_TESTS_EXPORT ConferenceCallWidget : public QWidget
{
    Q_OBJECT
public:
    struct LIBRUQOLAWIDGETS_TESTS_EXPORT ConferenceCallStart {
        bool useCamera = false;
        bool useMic = false;
    };
    explicit ConferenceCallWidget(QWidget *parent = nullptr);
    ~ConferenceCallWidget() override;

    [[nodiscard]] ConferenceCallStart conferenceCallInfo() const;
    void setConferenceCallInfo(const ConferenceCallStart &infoCallStart);

private:
    QToolButton *const mMicroButton;
    QToolButton *const mCameraButton;
};
