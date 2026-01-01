/*
  SPDX-FileCopyrightText: 2025-2026 Laurent Montel <montel@kde.org>

  SPDX-License-Identifier: GPL-2.0-or-later
*/

#include "e2edisablewidget.h"
#include <KLocalizedString>
#include <KSeparator>
#include <QLabel>
#include <QPushButton>
#include <QVBoxLayout>

using namespace Qt::Literals::StringLiterals;
E2eDisableWidget::E2eDisableWidget(QWidget *parent)
    : QWidget{parent}
{
    auto mainLayout = new QVBoxLayout(this);
    mainLayout->setObjectName("mainLayout"_L1);
    mainLayout->setContentsMargins({});

    auto label = new QLabel(
        i18n("Disabling E2EE will compromise the privacy of this groupe privé. Access to any encrypted content will be lost for all groupe privé members.\n"
             "Encryption can be re-enabled later. Proceed with caution.\n"
             "If no one is able to access the encrypted content you can reset encryption keys instead."),
        this);
    label->setObjectName("label"_L1);
    label->setWordWrap(true);
    mainLayout->addWidget(label);

    auto separator = new KSeparator(this);
    separator->setObjectName("separator"_L1);
    mainLayout->addWidget(separator);

    auto labelReset =
        new QLabel(i18n("Alternatively, resetting encryption keys will keep encryption enabled but access to previously encrypted content may be lost."), this);
    labelReset->setObjectName("labelReset"_L1);
    labelReset->setWordWrap(true);
    mainLayout->addWidget(labelReset);

    auto pushButton = new QPushButton(i18n("Reset encryption keys"), this);
    pushButton->setObjectName("pushButton"_L1);
    mainLayout->addWidget(pushButton);
    connect(pushButton, &QPushButton::clicked, this, []() {
        qDebug() << " Not implemented yet";
        // TODO
    });
    mainLayout->addStretch(1);
}

E2eDisableWidget::~E2eDisableWidget() = default;

#include "moc_e2edisablewidget.cpp"
