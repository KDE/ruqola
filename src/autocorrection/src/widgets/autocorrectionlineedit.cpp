/*
  SPDX-FileCopyrightText: 2022-2023 Laurent Montel <montel@kde.org>

  SPDX-License-Identifier: GPL-2.0-or-later
*/

#include "autocorrectionlineedit.h"
#include <QKeyEvent>
#include <QTextCursor>
#include <TextAutoCorrection/AutoCorrection>

using namespace TextAutoCorrection;
class TextAutoCorrection::AutoCorrectionLineEditPrivate
{
public:
    AutoCorrectionLineEditPrivate()
        : mAutoCorrection(new TextAutoCorrection::AutoCorrection())
    {
    }

    ~AutoCorrectionLineEditPrivate()
    {
        if (mNeedToDeleteAutoCorrection) {
            delete mAutoCorrection;
        }
    }

    TextAutoCorrection::AutoCorrection *mAutoCorrection = nullptr;
    bool mNeedToDeleteAutoCorrection = true;
};

AutoCorrectionLineEdit::AutoCorrectionLineEdit(QWidget *parent)
    : QPlainTextEdit(parent)
    , d(new TextAutoCorrection::AutoCorrectionLineEditPrivate)
{
    setTabChangesFocus(true);
    // widget may not be resized vertically
    setSizePolicy(QSizePolicy(QSizePolicy::MinimumExpanding, QSizePolicy::Fixed));
    setLineWrapMode(NoWrap);
    setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    document()->adjustSize();
    document()->setDocumentMargin(2);
}

AutoCorrectionLineEdit::~AutoCorrectionLineEdit() = default;

TextAutoCorrection::AutoCorrection *AutoCorrectionLineEdit::autocorrection() const
{
    return d->mAutoCorrection;
}

void AutoCorrectionLineEdit::setAutocorrection(AutoCorrection *autocorrect)
{
    d->mNeedToDeleteAutoCorrection = false;
    delete d->mAutoCorrection;
    d->mAutoCorrection = autocorrect;
}

void AutoCorrectionLineEdit::setAutocorrectionLanguage(const QString &language)
{
    TextAutoCorrection::AutoCorrectionSettings *settings = d->mAutoCorrection->autoCorrectionSettings();
    settings->setLanguage(language);
    d->mAutoCorrection->setAutoCorrectionSettings(settings);
}

void AutoCorrectionLineEdit::keyPressEvent(QKeyEvent *e)
{
    if (d->mAutoCorrection && d->mAutoCorrection->autoCorrectionSettings()->isEnabledAutoCorrection()) {
        if ((e->key() == Qt::Key_Space) || (e->key() == Qt::Key_Enter) || (e->key() == Qt::Key_Return)) {
            if (!textCursor().hasSelection()) {
                int position = textCursor().position();
                // no Html format in subject. => false
                const bool addSpace = d->mAutoCorrection->autocorrect(false, *document(), position);
                QTextCursor cur = textCursor();
                cur.setPosition(position);
                if (e->key() == Qt::Key_Space) {
                    if (addSpace) {
                        cur.insertText(QStringLiteral(" "));
                        setTextCursor(cur);
                    }
                    return;
                }
            }
        }
    }
    QPlainTextEdit::keyPressEvent(e);
}
