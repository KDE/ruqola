/*
   SPDX-FileCopyrightText: 2025 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "autogenerateinteractionuidialog.h"
#include "autogenerateinteractionui.h"

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
    auto view = new AutoGenerateInteractionUi(mRocketChatAccount, this);
    if (view->parseInteractionUi(r)) {
        auto w = view->generateWidget(this);
        connect(view, &AutoGenerateInteractionUi::closeCalled, this, &AutoGenerateInteractionUiDialog::reject);
        connect(view, &AutoGenerateInteractionUi::submitCalled, this, &AutoGenerateInteractionUiDialog::accept);
        mainLayout->addWidget(w);
        return true;
    } else {
        view->deleteLater();
        close();
    }
    return false;
}

#include "moc_autogenerateinteractionuidialog.cpp"
