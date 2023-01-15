/*
   SPDX-FileCopyrightText: 2023 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#pragma once

#include "libruqolawidgets_private_export.h"
#include <QWidget>

class QLineEdit;
class ServerErrorInfoMessageHistoryListView;
class LIBRUQOLAWIDGETS_TESTS_EXPORT ServerErrorInfoMessageHistoryWidget : public QWidget
{
    Q_OBJECT
public:
    explicit ServerErrorInfoMessageHistoryWidget(QWidget *parent = nullptr);
    ~ServerErrorInfoMessageHistoryWidget() override;

private:
#ifdef HAVE_TEXT_TO_SPEECH_SUPPORT
    void slotTextToSpeech(const QString &messageText);
#endif
    QLineEdit *const mSearchLineEdit;
    ServerErrorInfoMessageHistoryListView *const mListServerInfosListView;
};
