/*
   SPDX-FileCopyrightText: 2020-2026 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "userfeedbackmanager.h"
#include "ruqolauserfeedbackprovider.h"

UserFeedBackManager::UserFeedBackManager()
    : mUserFeedbackProvider(new RuqolaUserFeedbackProvider())
{
}

UserFeedBackManager::~UserFeedBackManager()
{
    delete mUserFeedbackProvider;
}

UserFeedBackManager *UserFeedBackManager::self()
{
    static UserFeedBackManager s_self;
    return &s_self;
}

KUserFeedback::Provider *UserFeedBackManager::userFeedbackProvider() const
{
    return mUserFeedbackProvider;
}
