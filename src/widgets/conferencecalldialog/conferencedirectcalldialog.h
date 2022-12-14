/*
   SPDX-FileCopyrightText: 2022 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#pragma once

#include "conferencecallwidget.h"
#include "libruqolawidgets_private_export.h"
#include <QDialog>
class RocketChatAccount;
class QToolButton;
class ConferenceCallWidget;
class LIBRUQOLAWIDGETS_TESTS_EXPORT ConferenceDirectCallDialog : public QDialog
{
    Q_OBJECT
public:
    explicit ConferenceDirectCallDialog(RocketChatAccount *account, QWidget *parent = nullptr);
    ~ConferenceDirectCallDialog() override;

    Q_REQUIRED_RESULT QString roomId() const;
    void setRoomId(const QString &newRoomId);

    Q_REQUIRED_RESULT bool allowRinging() const;
    void setAllowRinging(bool newAllowRinging);

private:
    void slotRejected();
    void slotStartVideoConference();
    QString mRoomId;
    ConferenceCallWidget *const mConferenceCallWidget;
    RocketChatAccount *const mRocketChatAccount;
    bool mAllowRinging = false;
};
