/*
   Copyright (C) 2020 Laurent Montel <montel@kde.org>

   This program is free software; you can redistribute it and/or
   modify it under the terms of the GNU General Public
   License as published by the Free Software Foundation; either
   version 2 of the License, or (at your option) any later version.

   This program is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
   General Public License for more details.

   You should have received a copy of the GNU General Public License
   along with this program; see the file COPYING.  If not, write to
   the Free Software Foundation, Inc., 51 Franklin Street, Fifth Floor,
   Boston, MA 02110-1301, USA.
*/

#include "ruqolauserfeedbackprovider.h"
#include "accountinfosource.h"

#include <KUserFeedback/ApplicationVersionSource>
#include <KUserFeedback/PlatformInfoSource>
#include <KUserFeedback/ScreenInfoSource>
#include <KUserFeedback/QtVersionSource>
#include <KUserFeedback/StartCountSource>
#include <KUserFeedback/UsageTimeSource>
#include <KUserFeedback/LocaleInfoSource>

RuqolaUserFeedbackProvider::RuqolaUserFeedbackProvider(QObject *parent)
    : KUserFeedback::Provider(parent)
{
    setProductIdentifier(QStringLiteral("org.kde.ruqola"));
    setFeedbackServer(QUrl(QStringLiteral("https://telemetry.kde.org/")));
    setSubmissionInterval(7);
    setApplicationStartsUntilEncouragement(5);
    setEncouragementDelay(30);

    addDataSource(new KUserFeedback::ApplicationVersionSource);
    addDataSource(new KUserFeedback::PlatformInfoSource);
    addDataSource(new KUserFeedback::ScreenInfoSource);
    addDataSource(new KUserFeedback::QtVersionSource);

    addDataSource(new KUserFeedback::StartCountSource);
    addDataSource(new KUserFeedback::UsageTimeSource);

    addDataSource(new KUserFeedback::LocaleInfoSource);

    addDataSource(new AccountInfoSource);
}

RuqolaUserFeedbackProvider::~RuqolaUserFeedbackProvider()
{
}
