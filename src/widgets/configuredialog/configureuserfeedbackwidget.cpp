/*
   SPDX-FileCopyrightText: 2020-2025 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "configureuserfeedbackwidget.h"
using namespace Qt::Literals::StringLiterals;

#include <QVBoxLayout>

#if HAVE_KUSERFEEDBACK
#include "userfeedback/userfeedbackmanager.h"
#include <KUserFeedback/FeedbackConfigWidget>
#include <KUserFeedback/Provider>
#endif

ConfigureUserFeedbackWidget::ConfigureUserFeedbackWidget(QWidget *parent)
    : QWidget(parent)
#if HAVE_KUSERFEEDBACK
    , mUserFeedbackWidget(new KUserFeedback::FeedbackConfigWidget(this))
#endif
{
    auto userFeedBackLayout = new QVBoxLayout(this);
    userFeedBackLayout->setObjectName(u"userFeedBackLayout"_s);
    userFeedBackLayout->setContentsMargins({});
#if HAVE_KUSERFEEDBACK
    mUserFeedbackWidget->setObjectName(u"mUserFeedbackWidget"_s);
    userFeedBackLayout->addWidget(mUserFeedbackWidget);
#endif
}

ConfigureUserFeedbackWidget::~ConfigureUserFeedbackWidget() = default;

void ConfigureUserFeedbackWidget::save()
{
#if HAVE_KUSERFEEDBACK
    // set current active mode + write back the config for future starts
    UserFeedBackManager::self()->userFeedbackProvider()->setTelemetryMode(mUserFeedbackWidget->telemetryMode());
    UserFeedBackManager::self()->userFeedbackProvider()->setSurveyInterval(mUserFeedbackWidget->surveyInterval());
#endif
}

void ConfigureUserFeedbackWidget::load()
{
#if HAVE_KUSERFEEDBACK
    mUserFeedbackWidget->setFeedbackProvider(UserFeedBackManager::self()->userFeedbackProvider());
#endif
}

void ConfigureUserFeedbackWidget::restoreToDefaults()
{
    // TODO
}

#include "moc_configureuserfeedbackwidget.cpp"
