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

    auto label = new QLabel(i18nc("@label:textbox", "Version:"), this);
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

QString WhatsNewComboBoxWidget::convertVersionEnumToString(WhatsNewUtils::VersionType type)
{
    switch (type) {
    case WhatsNewUtils::AllVersion:
        return i18n("All Version");
    case WhatsNewUtils::Version2_0:
        return i18n("Version 2.0");
    case WhatsNewUtils::Version2_1:
        return i18n("Version 2.1");
    case WhatsNewUtils::Version2_2:
        return i18n("Version 2.2");
    case WhatsNewUtils::Version2_3:
        return i18n("Version 2.3");
    }
    Q_UNREACHABLE();
    return {};
}

void WhatsNewComboBoxWidget::fillCombobox()
{
    for (int i = WhatsNewUtils::AllVersion; i <= WhatsNewUtils::LastVersion; ++i) {
        mVersionComboBox->addItem(convertVersionEnumToString(static_cast<WhatsNewUtils::VersionType>(i)), i);
    }
}

void WhatsNewComboBoxWidget::initializeVersion(WhatsNewUtils::VersionType type)
{
    const int index = mVersionComboBox->findData(type);
    if (index != -1) {
        mVersionComboBox->setCurrentIndex(index);
    }
}

void WhatsNewComboBoxWidget::slotCurrentIndexChanged(int index)
{
    const WhatsNewUtils::VersionType type = mVersionComboBox->itemData(index).value<WhatsNewUtils::VersionType>();
    Q_EMIT versionChanged(type);
}

#include "moc_whatsnewcomboboxwidget.cpp"
