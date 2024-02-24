/*
   SPDX-FileCopyrightText: 2023-2024 Laurent Montel <montel.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#pragma once

#include "accountmanager.h"
#include "libruqolawidgets_private_export.h"
#include <QWidget>

namespace TextEditTextToSpeech
{
class TextToSpeechContainerWidget;
}

class QLineEdit;
class ServerErrorInfoHistoryFilterProxyModel;
class ServerErrorInfoMessageHistoryListView;
class ServersComboBox;
class LIBRUQOLAWIDGETS_TESTS_EXPORT ServerErrorInfoMessageHistoryWidget : public QWidget
{
    Q_OBJECT
public:
    explicit ServerErrorInfoMessageHistoryWidget(QWidget *parent = nullptr);
    ~ServerErrorInfoMessageHistoryWidget() override;

    void addServerList(const QList<AccountManager::AccountDisplayInfo> &infos);

private:
    LIBRUQOLAWIDGETS_NO_EXPORT void slotTextChanged(const QString &str);
    LIBRUQOLAWIDGETS_NO_EXPORT void slotFilterAccount(const QString &accountName);

    QLineEdit *const mSearchLineEdit;
    ServerErrorInfoMessageHistoryListView *const mListServerInfosListView;
    TextEditTextToSpeech::TextToSpeechContainerWidget *const mTextToSpeechWidget = nullptr;
    ServerErrorInfoHistoryFilterProxyModel *const mServerErrorInfoHistoryFilterProxyModel;
    ServersComboBox *const mServersComboBox;
};
