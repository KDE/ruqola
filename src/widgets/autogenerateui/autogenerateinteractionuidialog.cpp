/*
   SPDX-FileCopyrightText: 2025 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "autogenerateinteractionuidialog.h"
#include "autogenerateinteractionui.h"
#include "ruqola_autogenerateui_debug.h"
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
    connect(mAutoGenerateInteractionUi, &AutoGenerateInteractionUi::actionChanged, this, &AutoGenerateInteractionUiDialog::slotActionChanged);
    mainLayout->setContentsMargins({});
    mainLayout->setObjectName("mainLayout"_L1);
}

AutoGenerateInteractionUiDialog::~AutoGenerateInteractionUiDialog() = default;

bool AutoGenerateInteractionUiDialog::parse(const QJsonObject &r)
{
    if (mAutoGenerateInteractionUi->parseInteractionUi(r)) {
        if (mMainWidget) {
            const QJsonObject values = mAutoGenerateInteractionUi->serializeState();
            // TODO use it
            mainLayout->removeWidget(mMainWidget);
            delete mMainWidget;
        }
        mMainWidget = mAutoGenerateInteractionUi->generateWidget(this);
        mainLayout->addWidget(mMainWidget);
        return true;
    }
    return false;
}

void AutoGenerateInteractionUiDialog::slotActionChanged(const QJsonObject &replyObject)
{
    if (!parse(replyObject)) {
        qCWarning(RUQOLA_AUTOGENERATEUI_LOG) << "Impossible to parse " << replyObject;
    }
}

#include "moc_autogenerateinteractionuidialog.cpp"
