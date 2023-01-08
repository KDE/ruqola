/*
   SPDX-FileCopyrightText: 2014-2023 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#pragma once
#include "textedittexttospeech_export.h"
#include <QObject>
namespace TextEditTextToSpeech
{
/**
 * @brief The AbstractTextToSpeechInterface class
 * @author Laurent Montel <montel@kde.org>
 */
class TEXTEDITTEXTTOSPEECH_EXPORT AbstractTextToSpeechInterface : public QObject
{
    Q_OBJECT
public:
    explicit AbstractTextToSpeechInterface(QObject *parent = nullptr);
    ~AbstractTextToSpeechInterface() override;

    Q_REQUIRED_RESULT virtual bool isReady() const;
    virtual void say(const QString &text);
    Q_REQUIRED_RESULT virtual double volume() const;
    virtual void setVolume(double value);
    virtual void reloadSettings();
};
}
