/*
   SPDX-FileCopyrightText: 2020-2025 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "channelpasswordwidget.h"
using namespace Qt::Literals::StringLiterals;

#include <KAuthorized>
#include <KLocalizedString>
#include <KPasswordLineEdit>
#include <QHBoxLayout>
#include <QLabel>

ChannelPasswordWidget::ChannelPasswordWidget(QWidget *parent)
    : QWidget(parent)
    , mPasswordLineEdit(new KPasswordLineEdit(this))
{
    auto mainLayout = new QHBoxLayout(this);
    mainLayout->setObjectName(u"mainLayout"_s);
    mainLayout->setContentsMargins({});

    auto label = new QLabel(i18nc("@label:textbox", "Channel Password:"), this);
    label->setTextFormat(Qt::PlainText);
    label->setObjectName(u"label"_s);
    mainLayout->addWidget(label);

    mPasswordLineEdit->setObjectName(u"mPasswordLineEdit"_s);
    mPasswordLineEdit->setRevealPasswordMode(KAuthorized::authorize(u"lineedit_reveal_password"_s) ? KPassword::RevealMode::OnlyNew
                                                                                                   : KPassword::RevealMode::Never);
    mainLayout->addWidget(mPasswordLineEdit);
    connect(mPasswordLineEdit, &KPasswordLineEdit::passwordChanged, this, &ChannelPasswordWidget::slotPasswordChanged);
}

ChannelPasswordWidget::~ChannelPasswordWidget() = default;

QString ChannelPasswordWidget::password() const
{
    return mPasswordLineEdit->password();
}

void ChannelPasswordWidget::slotPasswordChanged(const QString &str)
{
    Q_EMIT updateOkButton(!str.isEmpty());
}

#include "moc_channelpasswordwidget.cpp"
