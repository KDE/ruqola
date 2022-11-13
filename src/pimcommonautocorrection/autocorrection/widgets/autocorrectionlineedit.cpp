/*
  SPDX-FileCopyrightText: 2022 Laurent Montel <montel@kde.org>

  SPDX-License-Identifier: GPL-2.0-or-later
*/

#include "autocorrectionlineedit.h"
#include <PimCommonAutoCorrection/AutoCorrection>
#include <QKeyEvent>
#include <QTextCursor>

using namespace PimCommonAutoCorrection;
class PimCommonAutoCorrection::AutoCorrectionLineEditPrivate
{
public:
    AutoCorrectionLineEditPrivate()
        : mAutoCorrection(new PimCommonAutoCorrection::AutoCorrection())
    {
    }

    ~AutoCorrectionLineEditPrivate()
    {
        if (mNeedToDeleteAutoCorrection) {
            delete mAutoCorrection;
        }
    }

    PimCommonAutoCorrection::AutoCorrection *mAutoCorrection = nullptr;
    bool mNeedToDeleteAutoCorrection = true;
};

AutoCorrectionLineEdit::AutoCorrectionLineEdit(QWidget *parent)
    : QPlainTextEdit(parent)
    , d(new PimCommonAutoCorrection::AutoCorrectionLineEditPrivate)
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

PimCommonAutoCorrection::AutoCorrection *AutoCorrectionLineEdit::autocorrection() const
{
    return d->mAutoCorrection;
}

void AutoCorrectionLineEdit::setAutocorrection(PimCommonAutoCorrection::AutoCorrection *autocorrect)
{
    d->mNeedToDeleteAutoCorrection = false;
    delete d->mAutoCorrection;
    d->mAutoCorrection = autocorrect;
}

void AutoCorrectionLineEdit::setAutocorrectionLanguage(const QString &language)
{
    PimCommonAutoCorrection::AutoCorrectionSettings *settings = d->mAutoCorrection->autoCorrectionSettings();
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
