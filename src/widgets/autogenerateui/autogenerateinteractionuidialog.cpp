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
    , mAutoGenerateInteractionUi(new AutoGenerateInteractionUi(mRocketChatAccount, this))
{
    connect(mAutoGenerateInteractionUi, &AutoGenerateInteractionUi::closeCalled, this, &AutoGenerateInteractionUiDialog::reject);
    connect(mAutoGenerateInteractionUi, &AutoGenerateInteractionUi::submitCalled, this, &AutoGenerateInteractionUiDialog::accept);
    mainLayout->setContentsMargins({});
    mainLayout->setObjectName("mainLayout"_L1);
}

AutoGenerateInteractionUiDialog::~AutoGenerateInteractionUiDialog() = default;

bool AutoGenerateInteractionUiDialog::parse(const QJsonObject &r)
{
    if (mAutoGenerateInteractionUi->parseInteractionUi(r)) {
        if (mMainWidget) {
            mainLayout->removeWidget(mMainWidget);
            delete mMainWidget;
        }
        mMainWidget = mAutoGenerateInteractionUi->generateWidget(this);
        mainLayout->addWidget(mMainWidget);
        return true;
    } else {
        close();
    }
    return false;
}

#include "moc_autogenerateinteractionuidialog.cpp"
