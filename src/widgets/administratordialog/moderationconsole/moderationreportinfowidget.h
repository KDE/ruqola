/*
   SPDX-FileCopyrightText: 2023-2024 Laurent Montel <montel.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#pragma once

#include <QWidget>

#include "libruqolawidgets_private_export.h"
class ModerationReportInfoListView;
class QLineEdit;
class ModerationReportInfoModel;
class ModerationReportInfoFilterProxyModel;
class ModerationReportInfos;
class RocketChatAccount;
namespace TextEditTextToSpeech
{
class TextToSpeechContainerWidget;
}

class LIBRUQOLAWIDGETS_TESTS_EXPORT ModerationReportInfoWidget : public QWidget
{
    Q_OBJECT
public:
    explicit ModerationReportInfoWidget(RocketChatAccount *account, QWidget *parent = nullptr);
    ~ModerationReportInfoWidget() override;

    void setReportInfos(const ModerationReportInfos &infos);

private:
    LIBRUQOLAWIDGETS_NO_EXPORT void slotTextToSpeech(const QString &messageText);
    LIBRUQOLAWIDGETS_NO_EXPORT void slotCustomContextMenuRequested(const QPoint &pos);
    LIBRUQOLAWIDGETS_NO_EXPORT void slotTextChanged(const QString &str);
    ModerationReportInfoListView *const mListNotificationsListView;
    QLineEdit *const mSearchLineEdit;
    ModerationReportInfoFilterProxyModel *const mModerationReportInfoFilterProxyModel;
    ModerationReportInfoModel *const mModerationReportInfoModel;
    TextEditTextToSpeech::TextToSpeechContainerWidget *const mTextToSpeechWidget = nullptr;
};
