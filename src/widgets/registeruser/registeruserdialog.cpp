/*
   SPDX-FileCopyrightText: 2020-2026 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "registeruserdialog.h"

#include "registeruserwidget.h"
#include <KLocalizedString>
#include <QDialogButtonBox>
#include <QVBoxLayout>
#include <TextAddonsWidgets/LoadDialogSizeUtils>

namespace
{
const char myRegisterUserDialogConfigGroupName[] = "RegisterUserDialog";
}

using namespace Qt::Literals::StringLiterals;
RegisterUserDialog::RegisterUserDialog(QWidget *parent)
    : QDialog(parent)
    , mRegisterUserWidget(new RegisterUserWidget(this))
{
    setWindowTitle(i18nc("@title:window", "Register User"));
    auto mainLayout = new QVBoxLayout(this);
    mainLayout->setObjectName(u"mainLayout"_s);

    mRegisterUserWidget->setObjectName(u"mRegisterUserWidget"_s);
    mainLayout->addWidget(mRegisterUserWidget);

    auto button = new QDialogButtonBox(QDialogButtonBox::Close, this);
    button->setObjectName(u"button"_s);
    mainLayout->addWidget(button);
    connect(button, &QDialogButtonBox::rejected, this, &RegisterUserDialog::reject);
    connect(mRegisterUserWidget, &RegisterUserWidget::registerNewAccount, this, &RegisterUserDialog::registerNewAccount);
    readConfig();
}

RegisterUserDialog::~RegisterUserDialog() = default;

void RegisterUserDialog::readConfig()
{
    TextAddonsWidgets::LoadDialogSizeUtils::manageDialogSize(this, QLatin1StringView(myRegisterUserDialogConfigGroupName), QSize(400, 300));
}

RocketChatRestApi::RegisterUserJob::RegisterUserInfo RegisterUserDialog::registerUserInfo() const
{
    return mRegisterUserWidget->registerUserInfo();
}

void RegisterUserDialog::setPasswordValidChecks(const RuqolaServerConfig::PasswordSettings &passwordSettings)
{
    mRegisterUserWidget->setPasswordValidChecks(passwordSettings);
}

void RegisterUserDialog::setManuallyApproveNewUsersRequired(bool manual)
{
    mRegisterUserWidget->setManuallyApproveNewUsersRequired(manual);
}

#include "moc_registeruserdialog.cpp"
