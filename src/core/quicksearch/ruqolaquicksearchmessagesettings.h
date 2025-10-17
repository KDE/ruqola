/*
  SPDX-FileCopyrightText: 2025 Laurent Montel <montel@kde.org>

  SPDX-License-Identifier: GPL-2.0-or-later
*/

#pragma once
#include "libruqola_private_export.h"
#include <QByteArray>
#include <QObject>
class MessagesModel;
class LIBRUQOLACORE_TESTS_EXPORT RuqolaQuickSearchMessageSettings : public QObject
{
    Q_OBJECT
public:
    explicit RuqolaQuickSearchMessageSettings(MessagesModel *model, QObject *parent = nullptr);
    ~RuqolaQuickSearchMessageSettings() override;

    void next();
    void previous();

    [[nodiscard]] QByteArray currentMessageIdentifier() const;
    void setCurrentMessageIdentifier(const QByteArray &newCurrentMessageIdentifier);

    [[nodiscard]] int currentSearchIndex() const;
    void setCurrentSearchIndex(int newCurrentSearchIndex);

    void clear();
Q_SIGNALS:
    void refreshMessage(const QByteArray &identifier, const QByteArray &previousIdentifier, int index);
    void updateNextPreviousButtons(bool nextEnabled, bool previousEnabled);

private:
    [[nodiscard]] LIBRUQOLACORE_NO_EXPORT bool canSearchMessage() const;
    LIBRUQOLACORE_NO_EXPORT void lastMessageUuid();
    MessagesModel *const mMessageModel;
    QByteArray mCurrentMessageIdentifier;
    int mCurrentSearchIndex = -1;
};
