/*
   SPDX-FileCopyrightText: 2022 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#pragma once

#include "libruqolawidgets_private_export.h"
#include <QDialog>
class RocketChatAccount;
class ConferenceInfoWidget;
class LIBRUQOLAWIDGETS_TESTS_EXPORT ConferenceInfoDialog : public QDialog
{
    Q_OBJECT
public:
    explicit ConferenceInfoDialog(RocketChatAccount *account, QWidget *parent = nullptr);
    ~ConferenceInfoDialog() override;

private:
    ConferenceInfoWidget *const mConferenceInfoWidget;
};
