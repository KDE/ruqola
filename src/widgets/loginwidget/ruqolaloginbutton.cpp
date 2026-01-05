/*
   SPDX-FileCopyrightText: 2026 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/
#include "ruqolaloginbutton.h"
#include <KLocalizedString>

RuqolaLoginButton::RuqolaLoginButton(QWidget *parent)
    : QPushButton(parent)
{
    connect(this, &QPushButton::clicked, this, [this]() {
        if (mLoginInProgress) {
            Q_EMIT cancelLoginRequested();
        } else {
            Q_EMIT loginRequested();
        }
    });
}

RuqolaLoginButton::~RuqolaLoginButton() = default;

bool RuqolaLoginButton::loginInProgress() const
{
    return mLoginInProgress;
}

void RuqolaLoginButton::setLoginInProgress(bool newLoginInProgress)
{
    mLoginInProgress = newLoginInProgress;
    if (mLoginInProgress) {
        setText(i18nc("@action:button", "Cancel"));
    } else {
        setText(i18nc("@action:button", "Login"));
    }
}

#include "moc_ruqolaloginbutton.cpp"
