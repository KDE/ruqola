/*
  SPDX-FileCopyrightText: 2022 Laurent Montel <montel@kde.org>

  SPDX-License-Identifier: GPL-2.0-or-later
*/

#include "lingvaenginewidget.h"
#include <KLocalizedString>
#include <QFormLayout>
#include <QLineEdit>
#include <QVBoxLayout>

LingvaEngineWidget::LingvaEngineWidget(QWidget *parent)
    : QWidget{parent}
    , mServerUrl(new QLineEdit(this))
{
    auto mainLayout = new QFormLayout(this);
    mainLayout->setObjectName(QStringLiteral("mainLayout"));

    mServerUrl->setObjectName(QStringLiteral("mServerUrl"));
    mainLayout->addRow(i18n("Server Url:"), mServerUrl);
}

LingvaEngineWidget::~LingvaEngineWidget() = default;

QString LingvaEngineWidget::serverUrl() const
{
    return mServerUrl->text();
}

void LingvaEngineWidget::setServerUrl(const QString &serverUrl)
{
    mServerUrl->setText(serverUrl);
}
