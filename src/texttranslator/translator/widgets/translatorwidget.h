/*

  SPDX-FileCopyrightText: 2012-2023 Laurent Montel <montel@kde.org>

  SPDX-License-Identifier: GPL-2.0-or-later
*/

#pragma once

#include "texttranslator_export.h"
#include <QPlainTextEdit>
namespace TextTranslator
{
class TranslatorResultTextEdit : public QPlainTextEdit
{
    Q_OBJECT
public:
    explicit TranslatorResultTextEdit(QWidget *parent = nullptr);

    void setResultFailed(bool failed);

protected:
    void paintEvent(QPaintEvent *event) override;

private:
    bool mResultFailed = false;
};

/**
 * @brief The TranslatorTextEdit class
 * @author Laurent Montel <montel@kde.org>
 */
class TEXTTRANSLATOR_EXPORT TranslatorTextEdit : public QPlainTextEdit
{
    Q_OBJECT
public:
    explicit TranslatorTextEdit(QWidget *parent = nullptr);

Q_SIGNALS:
    void translateText();

protected:
    void dropEvent(QDropEvent *) override;
};

/**
 * @brief The TranslatorWidget class
 * @author Laurent Montel <montel@kde.org>
 */
class TEXTTRANSLATOR_EXPORT TranslatorWidget : public QWidget
{
    Q_OBJECT
public:
    explicit TranslatorWidget(QWidget *parent = nullptr);
    explicit TranslatorWidget(const QString &text, QWidget *parent = nullptr);
    ~TranslatorWidget() override;

    void setTextToTranslate(const QString &);
    void writeConfig();
    void readConfig();
    void setStandalone(bool b);

public Q_SLOTS:
    void slotTranslate();
    void slotCloseWidget();

private:
    void slotFromLanguageChanged(int, bool initialize = false);
    void slotTextChanged();
    void slotInvertLanguage();
    void slotClear();
    void slotTranslateDone();
    void slotTranslateFailed(bool result, const QString &message);
    void slotDebug();
    void slotConfigChanged();

protected:
    bool event(QEvent *e) override;

Q_SIGNALS:
    void toolsWasClosed();

private:
    void init();
    void initLanguage();
    void switchEngine();
    void loadEngineSettings();
    class TranslatorWidgetPrivate;
    std::unique_ptr<TranslatorWidgetPrivate> const d;
};
}
