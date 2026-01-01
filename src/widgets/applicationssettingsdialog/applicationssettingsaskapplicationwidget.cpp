/*
   SPDX-FileCopyrightText: 2024-2026 Laurent Montel <montel@kde.org>

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
    , mTextAppLabel(new QLabel(this))
    , mMessage(new KTextEdit(this))
{
    auto mainLayout = new QVBoxLayout(this);
    mainLayout->setObjectName("mainLayout"_L1);
    mainLayout->setContentsMargins({});

    mTextAppLabel->setObjectName("mTextAppLabel"_L1);
    QFont labFont = mTextAppLabel->font();
    labFont.setBold(true);
    mTextAppLabel->setFont(labFont);
    mainLayout->addWidget(mTextAppLabel);

    auto label = new QLabel(i18nc("@label:textbox", "Let your admin know why this app would be useful"), this);
    label->setObjectName("label"_L1);
    mainLayout->addWidget(label);

    mMessage->setObjectName("mMessage"_L1);
    mainLayout->addWidget(mMessage);
    mMessage->setPlaceholderText(i18nc("@info:placeholder", "Message for workspace admin"));
    connect(mMessage, &KTextEdit::textChanged, this, &ApplicationsSettingsAskApplicationWidget::slotUpdateOkButton);
}

ApplicationsSettingsAskApplicationWidget::~ApplicationsSettingsAskApplicationWidget() = default;

QString ApplicationsSettingsAskApplicationWidget::message() const
{
    return mMessage->toPlainText();
}

void ApplicationsSettingsAskApplicationWidget::setApplicationName(const QString &appName)
{
    mTextAppLabel->setText(i18n("Want to use %1", appName));
}

void ApplicationsSettingsAskApplicationWidget::slotUpdateOkButton()
{
    Q_EMIT updateOkButton(!mMessage->toPlainText().trimmed().isEmpty());
}

#include "moc_applicationssettingsaskapplicationwidget.cpp"
