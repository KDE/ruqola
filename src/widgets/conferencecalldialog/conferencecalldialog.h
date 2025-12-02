/*
   SPDX-FileCopyrightText: 2022-2025 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#pragma once

#include "conferencecallwidget.h"
#include "libruqolawidgets_private_export.h"

#include <QDialog>
class LIBRUQOLAWIDGETS_TESTS_EXPORT ConferenceCallDialog : public QDialog
{
    Q_OBJECT
public:
    explicit ConferenceCallDialog(QWidget *parent = nullptr);
    ~ConferenceCallDialog() override;

    [[nodiscard]] ConferenceCallWidget::ConferenceCallStart conferenceCallInfo() const;
    void setConferenceCallInfo(ConferenceCallWidget::ConferenceCallStart infoCallStart);

private:
    ConferenceCallWidget *const mConferenceCallWidget;
};
