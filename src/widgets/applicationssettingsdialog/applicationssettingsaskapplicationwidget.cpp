/*
   SPDX-FileCopyrightText: 2024 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "applicationssettingsaskapplicationwidget.h"
#include <KLocalizedString>
#include <KTextEdit>
#include <QLabel>
#include <QVBoxLayout>

using namespace Qt::Literals::StringLiterals;
ApplicationsSettingsAskApplicationWidget::ApplicationsSettingsAskApplicationWidget(QWidget *parent)
    : QWidget{parent}
    , mMessage(new KTextEdit(this))
{
    auto mainLayout = new QVBoxLayout(this);
    mainLayout->setObjectName("mainLayout"_L1);
    mainLayout->setContentsMargins({});

    auto label = new QLabel(i18n("Let your admin know why this app would be useful"), this);
    label->setObjectName("label"_L1);
    mainLayout->addWidget(label);

    mMessage->setObjectName("mMessage"_L1);
    mainLayout->addWidget(mMessage);
    mMessage->setPlaceholderText(i18n("Message for workspace admin"));
}

QString ApplicationsSettingsAskApplicationWidget::message() const
{
    return mMessage->toPlainText();
}

ApplicationsSettingsAskApplicationWidget::~ApplicationsSettingsAskApplicationWidget() = default;

#include "moc_applicationssettingsaskapplicationwidget.cpp"
