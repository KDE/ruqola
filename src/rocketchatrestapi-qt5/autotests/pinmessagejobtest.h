/*
   SPDX-FileCopyrightText: 2018-2021 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: SPDX-License-Identifier: GPL-2.0-or-later
*/
#pragma once

#include <QObject>

class PinMessageJobTest : public QObject
{
    Q_OBJECT
public:
    explicit PinMessageJobTest(QObject *parent = nullptr);
    ~PinMessageJobTest() override = default;
private Q_SLOTS:
    void shouldHaveDefaultValue();
    void shouldHaveMessageId();
    void shouldGeneratePinMessageRequest();
    void shouldGenerateUnPinMessageRequest();
    void shouldGenerateJson();
};

