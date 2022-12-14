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

private:
    ConferenceCallWidget *const mConferenceCallWidget;
};
