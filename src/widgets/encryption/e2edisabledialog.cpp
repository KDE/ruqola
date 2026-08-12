/*
  SPDX-FileCopyrightText: 2025-2026 Laurent Montel <montel@kde.org>

  SPDX-License-Identifier: GPL-2.0-or-later
*/
#include "e2edisabledialog.h"

#include "e2edisablewidget.h"
#include <KLocalizedString>
#include <QDialogButtonBox>
#include <QPushButton>
#include <QVBoxLayout>

using namespace Qt::Literals::StringLiterals;
E2eDisableDialog::E2eDisableDialog(RocketChatAccount *account, QWidget *parent)
    : QDialog(parent)
    , mE2eDisableWidget(new E2eDisableWidget(account, this))
{
    setWindowTitle(i18nc("@title:window", "Disable Encryption"));
    auto mainLayout = new QVBoxLayout(this);
    mainLayout->setObjectName(u"mainLayout"_s);

    mE2eDisableWidget->setObjectName(u"mE2eDisableWidget"_s);
    mainLayout->addWidget(mE2eDisableWidget);
    // Resetting the keys keeps encryption enabled: close without reporting a disable request.
    connect(mE2eDisableWidget, &E2eDisableWidget::resetEncryptionKeysDone, this, &E2eDisableDialog::reject);

    auto buttonBox = new QDialogButtonBox(QDialogButtonBox::Ok | QDialogButtonBox::Cancel, this);
    buttonBox->setObjectName(u"button"_s);
    connect(buttonBox, &QDialogButtonBox::accepted, this, &E2eDisableDialog::accept);
    connect(buttonBox, &QDialogButtonBox::rejected, this, &E2eDisableDialog::reject);
    buttonBox->button(QDialogButtonBox::Ok)->setText(i18nc("@action:button", "Disable Encryption"));
    mainLayout->addWidget(buttonBox);
}

E2eDisableDialog::~E2eDisableDialog() = default;

void E2eDisableDialog::setRoomId(const QByteArray &roomId)
{
    mE2eDisableWidget->setRoomId(roomId);
}

#include "moc_e2edisabledialog.cpp"
