/*
  SPDX-FileCopyrightText: 2024 Laurent Montel <montel.org>

  SPDX-License-Identifier: GPL-2.0-or-later
*/

#include "e2ecopypasswordwidget.h"
#include <KLocalizedString>
#include <QLabel>
#include <QVBoxLayout>

E2eCopyPasswordWidget::E2eCopyPasswordWidget(QWidget *parent)
    : QWidget{parent}
{
    auto mainLayout = new QVBoxLayout(this);
    mainLayout->setObjectName(QStringLiteral("mainLayout"));
    mainLayout->setContentsMargins({});
    auto label = new QLabel(this);
    label->setObjectName(QStringLiteral("label"));
    label->setTextFormat(Qt::RichText);
    QString randomPassword; // TODO
    label->setText(
        i18n("Create secure private rooms and direct messages with end-to-end encryption.<br/><br/>Save your password securely, as the key to encode/decode "
             "your messages won't be saved on the server. You'll need to enter it on other devices to use e2e encryption.<br/><br/>Change your password "
             "anytime from "
             "any browser you've entered it on. Remember to store your password before dismissing this message.<br/><br/>Your password is: <span "
             "style=\"font-weight: bold;\">%1</span>",
             randomPassword));
    mainLayout->addWidget(label);
}

E2eCopyPasswordWidget::~E2eCopyPasswordWidget() = default;

#include "moc_e2ecopypasswordwidget.cpp"
