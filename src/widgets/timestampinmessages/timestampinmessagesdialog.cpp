/*
   SPDX-FileCopyrightText: 2026 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/
#include "timestampinmessagesdialog.h"
#include <KLocalizedString>
#include <QDialogButtonBox>
#include <QPushButton>
#include <QVBoxLayout>
using namespace Qt::Literals::StringLiterals;
TimeStampInMessagesDialog::TimeStampInMessagesDialog(QWidget *parent)
    : QDialog(parent)
{
    setWindowTitle(i18nc("@title:window", "Add New WebDAV Server"));

    auto mainLayout = new QVBoxLayout(this);
    mainLayout->setObjectName(u"mainLayout"_s);

    /*
    mWebDavAddServerWidget->setObjectName(u"mWebDavAddServerWidget"_s);
    mainLayout->addWidget(mWebDavAddServerWidget);
*/
    auto buttonBox = new QDialogButtonBox(QDialogButtonBox::Ok | QDialogButtonBox::Cancel, this);
    buttonBox->setObjectName(u"buttonBox"_s);
    mainLayout->addWidget(buttonBox);
    auto buttonOk = buttonBox->button(QDialogButtonBox::Ok);
    buttonOk->setText(i18nc("@action:button", "Add WebDAV Server"));
    connect(buttonBox, &QDialogButtonBox::rejected, this, &TimeStampInMessagesDialog::reject);
    connect(buttonBox, &QDialogButtonBox::accepted, this, &TimeStampInMessagesDialog::accept);
    buttonOk->setEnabled(false);
}

TimeStampInMessagesDialog::~TimeStampInMessagesDialog() = default;

#include "moc_timestampinmessagesdialog.cpp"
