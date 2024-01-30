/*
   SPDX-FileCopyrightText: 2022-2024 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#pragma once

#include "conferencecallwidget.h"
#include "libruqolawidgets_private_export.h"
#include "videoconference/videoconference.h"
#include <QDialog>
class RocketChatAccount;
class ConferenceCallWidget;
class LIBRUQOLAWIDGETS_TESTS_EXPORT ConferenceDirectCallDialog : public QDialog
{
    Q_OBJECT
public:
    explicit ConferenceDirectCallDialog(RocketChatAccount *account, QWidget *parent = nullptr);
    ~ConferenceDirectCallDialog() override;

    [[nodiscard]] QString roomId() const;
    void setRoomId(const QString &newRoomId);

    [[nodiscard]] bool allowRinging() const;
    void setAllowRinging(bool newAllowRinging);

private:
    LIBRUQOLAWIDGETS_NO_EXPORT void cancelCall();
    LIBRUQOLAWIDGETS_NO_EXPORT void slotRejected();
    LIBRUQOLAWIDGETS_NO_EXPORT void slotStartVideoConference();
    LIBRUQOLAWIDGETS_NO_EXPORT void callUser();
    LIBRUQOLAWIDGETS_NO_EXPORT void slotVideoConferenceAccepted(const VideoConference &videoConference);
    LIBRUQOLAWIDGETS_NO_EXPORT void slotVideoConferenceCanceled(const VideoConference &videoConference);
    QString mRoomId;
    QString mCallId;
    ConferenceCallWidget *const mConferenceCallWidget;
    RocketChatAccount *const mRocketChatAccount;
    QPushButton *mOkButton = nullptr;
    int mNumberOfCall = 0;
    bool mAllowRinging = false;
    bool mWasAccepted = false;
};
