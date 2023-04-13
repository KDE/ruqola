/*
    SPDX-FileCopyrightText: 2023 Laurent Montel <montel@kde.org>

    SPDX-License-Identifier: LGPL-2.0-or-later
*/
#pragma once

#include <TextEmoticonsCore/CustomEmojiIconManager>

class CustomEmojiIconManagerGuiTest : public TextEmoticonsCore::CustomEmojiIconManager
{
    Q_OBJECT
public:
    explicit CustomEmojiIconManagerGuiTest(QObject *parent = nullptr);
    ~CustomEmojiIconManagerGuiTest() override;

    Q_REQUIRED_RESULT QIcon generateIcon(const QString &customIdentifier) override;
};
