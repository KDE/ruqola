/*
   SPDX-FileCopyrightText: 2022 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "conferencecalldialog.h"
#include "conferencecallwidget.h"

ConferenceCallDialog::ConferenceCallDialog(QWidget *parent)
    : QDialog(parent)
    , mConferenceCallWidget(new ConferenceCallWidget(this))
{
}

ConferenceCallDialog::~ConferenceCallDialog() = default;
