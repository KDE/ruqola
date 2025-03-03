/*
   SPDX-FileCopyrightText: 2025 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "autogenerateinteractionuidialog.h"
#include "autogenerateui/autogenerateinteractionui.h"

#include <QVBoxLayout>

using namespace Qt::Literals::StringLiterals;
AutoGenerateInteractionUiDialog::AutoGenerateInteractionUiDialog(RocketChatAccount *account, QWidget *parent)
    : QDialog(parent)
    , mainLayout(new QVBoxLayout(this))
    , mRocketChatAccount(account)
{
    mainLayout->setContentsMargins({});
    mainLayout->setObjectName("mainLayout"_L1);
}

AutoGenerateInteractionUiDialog::~AutoGenerateInteractionUiDialog() = default;

bool AutoGenerateInteractionUiDialog::parse(const QJsonObject &r)
{
    auto job = new AutoGenerateInteractionUi(this);
    if (job->parseInteractionUi(r)) {
        auto w = job->generateWidget(this, mRocketChatAccount);
        mainLayout->addWidget(w);
        return true;
    } else {
        job->deleteLater();
    }
    return false;
}
