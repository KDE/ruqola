/*
   SPDX-FileCopyrightText: 2022-2023 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "configureautocorrectionwidget.h"
#include "ruqola.h"
#include "textautocorrection/autocorrection.h"
#include "textautocorrection/autocorrectionwidget.h"
#include "textautocorrection/textautocorrectionsettings.h"
#include <QVBoxLayout>

ConfigureAutoCorrectionWidget::ConfigureAutoCorrectionWidget(QWidget *parent)
    : QWidget{parent}
    , mAutoCorrectionWidget(new TextAutoCorrection::AutoCorrectionWidget(this))
{
    auto mainLayout = new QVBoxLayout(this);
    mainLayout->setObjectName(QStringLiteral("mainLayout"));
    mainLayout->setContentsMargins({});
    mainLayout->addWidget(mAutoCorrectionWidget);
    // We don't have html support here.
    mAutoCorrectionWidget->setHasHtmlSupport(false);
    mAutoCorrectionWidget->setAutoCorrection(Ruqola::self()->autoCorrection());
}

ConfigureAutoCorrectionWidget::~ConfigureAutoCorrectionWidget() = default;

void ConfigureAutoCorrectionWidget::save()
{
    mAutoCorrectionWidget->writeConfig();
    TextAutoCorrection::TextAutoCorrectionSettings::self()->save();
}

void ConfigureAutoCorrectionWidget::load()
{
    mAutoCorrectionWidget->loadConfig();
}
