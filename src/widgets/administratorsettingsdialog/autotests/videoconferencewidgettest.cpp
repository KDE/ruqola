/*
   SPDX-FileCopyrightText: 2022 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "videoconferencewidgettest.h"
#include "administratorsettingsdialog/videoconference/videoconferencewidget.h"
#include <QTest>
QTEST_MAIN(VideoConferenceWidgetTest)
VideoConferenceWidgetTest::VideoConferenceWidgetTest(QObject *parent)
    : QObject{parent}
{
}

void VideoConferenceWidgetTest::shouldHaveDefaultValues()
{
    VideoConferenceWidget w(nullptr);
    // TODO
}
