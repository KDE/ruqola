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
