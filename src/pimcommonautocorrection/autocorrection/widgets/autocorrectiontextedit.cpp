/*
  SPDX-FileCopyrightText: 2022 Laurent Montel <montel@kde.org>

  SPDX-License-Identifier: GPL-2.0-or-later
*/

#include "autocorrectiontextedit.h"
#include <PimCommonAutoCorrection/AutoCorrection>

#include <QKeyEvent>

using namespace PimCommonAutoCorrection;

class PimCommonAutoCorrection::AutoCorrectionTextEditPrivate
{
public:
    AutoCorrectionTextEditPrivate()
        : mAutoCorrection(new PimCommonAutoCorrection::AutoCorrection())
    {
    }

    ~AutoCorrectionTextEditPrivate()
    {
        if (mNeedToDelete) {
            delete mAutoCorrection;
        }
    }

    PimCommonAutoCorrection::AutoCorrection *mAutoCorrection = nullptr;
    bool mNeedToDelete = true;
};

AutoCorrectionTextEdit::AutoCorrectionTextEdit(QWidget *parent)
    : QTextEdit(parent)
    , d(new PimCommonAutoCorrection::AutoCorrectionTextEditPrivate)
{
}

AutoCorrectionTextEdit::~AutoCorrectionTextEdit() = default;

void AutoCorrectionTextEdit::setAutocorrection(PimCommonAutoCorrection::AutoCorrection *autocorrect)
{
    d->mNeedToDelete = false;
    delete d->mAutoCorrection;
    d->mAutoCorrection = autocorrect;
}

PimCommonAutoCorrection::AutoCorrection *AutoCorrectionTextEdit::autocorrection() const
{
    return d->mAutoCorrection;
}

void AutoCorrectionTextEdit::setAutocorrectionLanguage(const QString &language)
{
    PimCommonAutoCorrection::AutoCorrectionSettings *settings = d->mAutoCorrection->autoCorrectionSettings();
    settings->setLanguage(language);
    d->mAutoCorrection->setAutoCorrectionSettings(settings);
}

static bool isSpecial(const QTextCharFormat &charFormat)
{
    return charFormat.isFrameFormat() || charFormat.isImageFormat() || charFormat.isListFormat() || charFormat.isTableFormat()
        || charFormat.isTableCellFormat();
}

void AutoCorrectionTextEdit::keyPressEvent(QKeyEvent *e)
{
    if (d->mAutoCorrection && d->mAutoCorrection->autoCorrectionSettings()->isEnabledAutoCorrection()) {
        if ((e->key() == Qt::Key_Space) || (e->key() == Qt::Key_Enter) || (e->key() == Qt::Key_Return)) {
            if (!textCursor().hasSelection()) {
                const QTextCharFormat initialTextFormat = textCursor().charFormat();
                const bool richText = acceptRichText();
                int position = textCursor().position();
                const bool addSpace = d->mAutoCorrection->autocorrect(richText, *document(), position);
                QTextCursor cur = textCursor();
                cur.setPosition(position);
                const bool spacePressed = (e->key() == Qt::Key_Space);
                const QChar insertChar = spacePressed ? QLatin1Char(' ') : QLatin1Char('\n');
                if (richText && !isSpecial(initialTextFormat)) {
                    if (addSpace || !spacePressed) {
                        cur.insertText(insertChar, initialTextFormat);
                    }
                } else {
                    if (addSpace || !spacePressed) {
                        cur.insertText(insertChar);
                    }
                }
                setTextCursor(cur);
                return;
            }
        }
    }
    QTextEdit::keyPressEvent(e);
}
