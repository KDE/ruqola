/*
  SPDX-FileCopyrightText: 2024-2026 Laurent Montel <montel@kde.org>

  SPDX-License-Identifier: GPL-2.0-or-later
*/

#include "forwardmessagedialog.h"
using namespace Qt::Literals::StringLiterals;

#include "forwardmessagewidget.h"
#include <KLocalizedString>
#include <QDialogButtonBox>
#include <QPushButton>
#include <QVBoxLayout>

ForwardMessageDialog::ForwardMessageDialog(RocketChatAccount *account, QWidget *parent)
    : QDialog(parent)
    , mForwardMessageWidget(new ForwardMessageWidget(account, this))
{
    setWindowTitle(i18nc("@title:window", "Forward Message"));
    auto mainLayout = new QVBoxLayout(this);
    mainLayout->setObjectName(u"mainLayout"_s);

    mForwardMessageWidget->setObjectName(u"mForwardMessageWidget"_s);
    mainLayout->addWidget(mForwardMessageWidget);

    auto buttonBox = new QDialogButtonBox(QDialogButtonBox::Ok | QDialogButtonBox::Cancel, this);
    buttonBox->setObjectName(u"button"_s);
    connect(buttonBox, &QDialogButtonBox::accepted, this, &ForwardMessageDialog::accept);
    connect(buttonBox, &QDialogButtonBox::rejected, this, &ForwardMessageDialog::reject);
    mainLayout->addWidget(buttonBox);

    auto buttonOk = buttonBox->button(QDialogButtonBox::Ok);
    buttonOk->setEnabled(false);
    connect(mForwardMessageWidget, &ForwardMessageWidget::updateOkButton, this, [buttonOk](bool enabled) {
        buttonOk->setEnabled(enabled);
    });
}

ForwardMessageDialog::~ForwardMessageDialog() = default;

QList<QByteArray> ForwardMessageDialog::channelIdentifiers() const
{
    return mForwardMessageWidget->channelIdentifiers();
}

#include "moc_forwardmessagedialog.cpp"
