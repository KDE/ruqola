/*
   SPDX-FileCopyrightText: 2020-2023 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "channelpasswordwidget.h"
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
    mainLayout->setObjectName(QStringLiteral("mainLayout"));
    mainLayout->setContentsMargins({});

    auto label = new QLabel(i18n("Channel Password:"), this);
    label->setTextFormat(Qt::PlainText);
    label->setObjectName(QStringLiteral("label"));
    mainLayout->addWidget(label);

    mPasswordLineEdit->setObjectName(QStringLiteral("mPasswordLineEdit"));
    mPasswordLineEdit->setRevealPasswordAvailable(KAuthorized::authorize(QStringLiteral("lineedit_reveal_password")));
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
