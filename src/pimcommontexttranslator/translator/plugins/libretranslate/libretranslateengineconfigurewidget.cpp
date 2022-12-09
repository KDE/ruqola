/*
  SPDX-FileCopyrightText: 2022 Laurent Montel <montel@kde.org>

  SPDX-License-Identifier: GPL-2.0-or-later
*/

#include "libretranslateengineconfigurewidget.h"
#include <KLocalizedString>
#include <QFormLayout>
#include <QLabel>
#include <QLineEdit>

LibreTranslateEngineConfigureWidget::LibreTranslateEngineConfigureWidget(QWidget *parent)
    : QWidget{parent}
    , mServerUrl(new QLineEdit(this))
{
    auto mainLayout = new QFormLayout(this);
    mainLayout->setObjectName(QStringLiteral("mainLayout"));

    mServerUrl->setObjectName(QStringLiteral("mServerUrl"));
    mServerUrl->setClearButtonEnabled(true);
    mainLayout->addRow(i18n("Server Url:"), mServerUrl);
}

LibreTranslateEngineConfigureWidget::~LibreTranslateEngineConfigureWidget() = default;

QString LibreTranslateEngineConfigureWidget::serverUrl() const
{
    return mServerUrl->text();
}

void LibreTranslateEngineConfigureWidget::setServerUrl(const QString &serverUrl)
{
    mServerUrl->setText(serverUrl);
}
