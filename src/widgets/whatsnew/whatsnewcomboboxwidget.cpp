/*
   SPDX-FileCopyrightText: 2023 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "whatsnewcomboboxwidget.h"
#include <KLocalizedString>
#include <KSeparator>
#include <QComboBox>
#include <QHBoxLayout>
#include <QLabel>

WhatsNewComboBoxWidget::WhatsNewComboBoxWidget(QWidget *parent)
    : QWidget{parent}
    , mVersionComboBox(new QComboBox(this))
{
    auto mainLayout = new QVBoxLayout(this);
    mainLayout->setObjectName(QStringLiteral("mainLayout"));
    mainLayout->setContentsMargins({});

    auto hboxLayout = new QHBoxLayout;
    hboxLayout->setObjectName(QStringLiteral("hboxLayout"));
    hboxLayout->setContentsMargins({});
    mainLayout->addLayout(hboxLayout);

    auto label = new QLabel(i18n("Version:"), this);
    label->setObjectName(QStringLiteral("label"));
    hboxLayout->addWidget(label);

    mVersionComboBox->setObjectName(QStringLiteral("mVersionComboBox"));
    hboxLayout->addWidget(mVersionComboBox);
    hboxLayout->addStretch(1);

    auto separator = new KSeparator(this);
    separator->setObjectName(QStringLiteral("separator"));
    mainLayout->addWidget(separator);

    fillCombobox();
    connect(mVersionComboBox, &QComboBox::currentIndexChanged, this, &WhatsNewComboBoxWidget::slotCurrentIndexChanged);
}

WhatsNewComboBoxWidget::~WhatsNewComboBoxWidget() = default;

void WhatsNewComboBoxWidget::fillCombobox()
{
    mVersionComboBox->addItem(i18n("All Version"), AllVersion);
    mVersionComboBox->addItem(i18n("2.0"), Version2_0);
}

void WhatsNewComboBoxWidget::initializeVersion()
{
    const int index = mVersionComboBox->findData(Version2_0);
    if (index != -1) {
        mVersionComboBox->setCurrentIndex(index);
    }
}

void WhatsNewComboBoxWidget::slotCurrentIndexChanged(int index)
{
    const VersionType type = mVersionComboBox->itemData(index).value<VersionType>();
    Q_EMIT versionChanged(type);
}

#include "moc_whatsnewcomboboxwidget.cpp"
