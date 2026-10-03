/*
   SPDX-FileCopyrightText: 2022-2026 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "configureautocorrectionwidget.h"
using namespace Qt::Literals::StringLiterals;

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
    mainLayout->setObjectName(u"mainLayout"_s);
    mainLayout->setContentsMargins({});
#if HAVE_TEXT_AUTOCORRECTION_WIDGETS
    mainLayout->addWidget(mAutoCorrectionWidget);
    // We don't have html support here.
    mAutoCorrectionWidget->setHasHtmlSupport(false);
    mAutoCorrectionWidget->setAutoCorrection(Ruqola::self()->autoCorrection());
#endif
}

ConfigureAutoCorrectionWidget::~ConfigureAutoCorrectionWidget() = default;

void ConfigureAutoCorrectionWidget::save()
{
#if HAVE_TEXT_AUTOCORRECTION_WIDGETS
    mAutoCorrectionWidget->writeConfig();
    TextAutoCorrectionCore::TextAutoCorrectionSettings::self()->save();
#endif
}

void ConfigureAutoCorrectionWidget::load()
{
#if HAVE_TEXT_AUTOCORRECTION_WIDGETS
    mAutoCorrectionWidget->loadConfig();
#endif
}

void ConfigureAutoCorrectionWidget::restoreToDefaults()
{
    // TODO
}

#include "moc_configureautocorrectionwidget.cpp"
