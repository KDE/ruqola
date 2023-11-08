/*
   SPDX-FileCopyrightText: 2022-2023 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "configureautocorrectionwidget.h"
#include "ruqola.h"

#if HAVE_TEXT_AUTOCORRECTION_WIDGETS
#include "textautocorrectioncore/textautocorrectionsettings.h"
#include <TextAutoCorrectionCore/AutoCorrection>
#include <TextAutoCorrectionWidgets/AutoCorrectionWidget>
#endif

#include <QVBoxLayout>

ConfigureAutoCorrectionWidget::ConfigureAutoCorrectionWidget(QWidget *parent)
    : QWidget{parent}
#if HAVE_TEXT_AUTOCORRECTION_WIDGETS
    , mAutoCorrectionWidget(new TextAutoCorrectionWidgets::AutoCorrectionWidget(this))
#endif
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
#if HAVE_TEXT_AUTOCORRECTION_WIDGETS
    TextAutoCorrectionCore::TextAutoCorrectionSettings::self()->save();
#endif
}

void ConfigureAutoCorrectionWidget::load()
{
    mAutoCorrectionWidget->loadConfig();
}

#include "moc_configureautocorrectionwidget.cpp"
