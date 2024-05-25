/*
   SPDX-FileCopyrightText: 2023-2024 Laurent Montel <montel.org>

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

QString WhatsNewComboBoxWidget::convertVersionEnumToString(WhatsNewComboBoxWidget::VersionType type)
{
    switch (type) {
    case AllVersion:
        return i18n("All Version");
    case Version2_0:
        return i18n("Version 2.0");
    case Version2_1:
        return i18n("Version 2.1");
    case Version2_2:
        return i18n("Version 2.2");
    case Version2_3:
        return i18n("Version 2.3");
    }
    return {};
}

void WhatsNewComboBoxWidget::fillCombobox()
{
    for (int i = AllVersion; i <= LastVersion; ++i) {
        mVersionComboBox->addItem(convertVersionEnumToString(static_cast<VersionType>(i)), i);
    }
}

void WhatsNewComboBoxWidget::initializeVersion(WhatsNewComboBoxWidget::VersionType type)
{
    const int index = mVersionComboBox->findData(type);
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
