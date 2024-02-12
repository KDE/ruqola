/*
   SPDX-FileCopyrightText: 2020-2024 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "channelpasswordwidget.h"
#include <KAuthorized>
#include <KLocalizedString>
#include <KPasswordLineEdit>
#include <QHBoxLayout>
#include <QLabel>
#include <kwidgetsaddons_version.h>

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
#if KWIDGETSADDONS_VERSION < QT_VERSION_CHECK(5, 249, 0)
    mPasswordLineEdit->setRevealPasswordAvailable(KAuthorized::authorize(QStringLiteral("lineedit_reveal_password")));
#else
    mPasswordLineEdit->setRevealPasswordMode(KAuthorized::authorize(QStringLiteral("lineedit_reveal_password")) ? KPassword::RevealMode::OnlyNew
                                                                                                                : KPassword::RevealMode::Never);
#endif
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
