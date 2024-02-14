/*
  SPDX-FileCopyrightText: 2024 Laurent Montel <montel.org>

  SPDX-License-Identifier: GPL-2.0-or-later
*/

#include "forwardmessagedialog.h"
#include "forwardmessagewidget.h"
#include <KLocalizedString>
#include <QDialogButtonBox>
#include <QVBoxLayout>

ForwardMessageDialog::ForwardMessageDialog(QWidget *parent)
    : QDialog(parent)
    , mForwardMessageWidget(new ForwardMessageWidget(this))
{
    setWindowTitle(i18nc("@title:window", "Forward Message"));
    auto mainLayout = new QVBoxLayout(this);
    mainLayout->setObjectName(QStringLiteral("mainLayout"));

    mForwardMessageWidget->setObjectName(QStringLiteral("mForwardMessageWidget"));
    mainLayout->addWidget(mForwardMessageWidget);

    auto buttonBox = new QDialogButtonBox(QDialogButtonBox::Close, this);
    buttonBox->setObjectName(QStringLiteral("button"));
    connect(buttonBox, &QDialogButtonBox::accepted, this, &ForwardMessageDialog::accept);
    connect(buttonBox, &QDialogButtonBox::rejected, this, &ForwardMessageDialog::reject);
    mainLayout->addWidget(buttonBox);
}

ForwardMessageDialog::~ForwardMessageDialog() = default;

#include "moc_forwardmessagedialog.cpp"
