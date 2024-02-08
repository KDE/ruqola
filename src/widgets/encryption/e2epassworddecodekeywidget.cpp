/*
  SPDX-FileCopyrightText: 2024 Laurent Montel <montel.org>

  SPDX-License-Identifier: GPL-2.0-or-later
*/

#include "e2epassworddecodekeywidget.h"
#include <KLocalizedString>
#include <KPasswordLineEdit>
#include <QLabel>
#include <QVBoxLayout>

E2ePasswordDecodeKeyWidget::E2ePasswordDecodeKeyWidget(QWidget *parent)
    : QWidget{parent}
    , mPassword(new KPasswordLineEdit(this))
{
    auto mainLayout = new QVBoxLayout(this);
    mainLayout->setObjectName(QStringLiteral("mainLayout"));
    mainLayout->setContentsMargins({});

    auto label =
        new QLabel(i18n("To access your encrypted private groups and direct messages, enter your encryption password.\n"
                        "You need to enter this password to encode/decode your messages on every client you use, since the key is not stored on the server."),
                   this);
    label->setObjectName(QStringLiteral("label"));
    label->setWordWrap(true);
    mainLayout->addWidget(label);
    mPassword->setObjectName(QStringLiteral("mPassword"));
    mainLayout->addWidget(mPassword);
}

E2ePasswordDecodeKeyWidget::~E2ePasswordDecodeKeyWidget() = default;

QString E2ePasswordDecodeKeyWidget::password() const
{
    return mPassword->password();
}

#include "moc_e2epassworddecodekeywidget.cpp"
