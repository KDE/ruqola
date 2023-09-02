/*
   SPDX-FileCopyrightText: 2023 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "whatsnewcomboboxwidget.h"
#include <KLocalizedString>
#include <QComboBox>
#include <QHBoxLayout>
#include <QLabel>

WhatsNewComboBoxWidget::WhatsNewComboBoxWidget(QWidget *parent)
    : QWidget{parent}
    , mVersionComboBox(new QComboBox(this))
{
    auto mainLayout = new QHBoxLayout(this);
    mainLayout->setObjectName(QStringLiteral("mainLayout"));
    mainLayout->setContentsMargins({});

    auto label = new QLabel(i18n("Version:"), this);
    label->setObjectName(QStringLiteral("label"));
    mainLayout->addWidget(label);

    mVersionComboBox->setObjectName(QStringLiteral("mVersionComboBox"));
    mainLayout->addWidget(mVersionComboBox);
    mainLayout->addStretch(1);
}

WhatsNewComboBoxWidget::~WhatsNewComboBoxWidget() = default;

#include "moc_whatsnewcomboboxwidget.cpp"
