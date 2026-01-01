/*
  SPDX-FileCopyrightText: 2024-2026 Laurent Montel <montel@kde.org>

  SPDX-License-Identifier: GPL-2.0-or-later
*/

#include "e2epassworddecodekeywidget.h"
using namespace Qt::Literals::StringLiterals;

#include <KAuthorized>
#include <KLineEditEventHandler>
#include <KLocalizedString>
#include <KPasswordLineEdit>
#include <QLabel>
#include <QVBoxLayout>

E2ePasswordDecodeKeyWidget::E2ePasswordDecodeKeyWidget(QWidget *parent)
    : QWidget{parent}
    , mPassword(new KPasswordLineEdit(this))
{
    auto mainLayout = new QVBoxLayout(this);
    mainLayout->setObjectName(u"mainLayout"_s);
    mainLayout->setContentsMargins({});

    mPassword->setRevealPasswordMode(KAuthorized::authorize(u"lineedit_reveal_password"_s) ? KPassword::RevealMode::OnlyNew : KPassword::RevealMode::Never);

    mPassword->lineEdit()->setPlaceholderText(i18nc("@info:placeholder", "Please enter your E2EE password"));
    KLineEditEventHandler::catchReturnKey(mPassword->lineEdit());
    auto label =
        new QLabel(i18nc("@label:textbox",
                         "To access your encrypted private groups and direct messages, enter your encryption password.\n"
                         "You need to enter this password to encode/decode your messages on every client you use, since the key is not stored on the server."),
                   this);
    label->setObjectName(u"label"_s);
    label->setWordWrap(true);
    mainLayout->addWidget(label);
    mPassword->setObjectName(u"mPassword"_s);
    mainLayout->addWidget(mPassword);
    mainLayout->addStretch(1);
}

E2ePasswordDecodeKeyWidget::~E2ePasswordDecodeKeyWidget() = default;

QString E2ePasswordDecodeKeyWidget::password() const
{
    return mPassword->password();
}

#include "moc_e2epassworddecodekeywidget.cpp"
