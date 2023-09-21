/*
   SPDX-FileCopyrightText: 2022-2023 Laurent Montel <montel@kde.org>

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

    void initializeInfo();

    [[nodiscard]] QString conferenceId() const;
    void setConferenceId(const QString &newConferenceId);

private:
    ConferenceInfoWidget *const mConferenceInfoWidget;
};
