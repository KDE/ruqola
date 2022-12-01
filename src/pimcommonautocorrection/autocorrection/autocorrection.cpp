/*
  SPDX-FileCopyrightText: 2012-2022 Laurent Montel <montel@kde.org>
  code based on calligra autocorrection.

  SPDX-License-Identifier: GPL-2.0-or-later
*/

#include "autocorrection.h"

#include "autocorrection/autocorrectionutils.h"
#include "pimcommonautocorrectionautocorrect_debug.h"
#include <KColorScheme>
#include <QFile>
#include <QLocale>
#include <QStandardPaths>
#include <QTextBlock>
#include <QTextDocument>

using namespace PimCommonAutoCorrection;

namespace PimCommonAutoCorrection
{
class AutoCorrectionPrivate
{
public:
    AutoCorrectionPrivate()
        : mAutoCorrectionSettings(new AutoCorrectionSettings)
    {
        const auto locale = QLocale::system();
        mCacheNameOfDays.reserve(7);
        for (int i = 1; i <= 7; ++i) {
            mCacheNameOfDays.append(locale.dayName(i).toLower());
        }
    }
    ~AutoCorrectionPrivate()
    {
        delete mAutoCorrectionSettings;
    }
    QString mWord;
    QTextCursor mCursor;

    QStringList mCacheNameOfDays;
    QColor mLinkColor;
    AutoCorrectionSettings *mAutoCorrectionSettings = nullptr;
};
}

AutoCorrection::AutoCorrection()
    : d(new AutoCorrectionPrivate())
{
}

AutoCorrection::~AutoCorrection() = default;

void AutoCorrection::selectStringOnMaximumSearchString(QTextCursor &cursor, int cursorPosition)
{
    cursor.setPosition(cursorPosition);

    QTextBlock block = cursor.block();
    int pos = qMax(block.position(), cursorPosition - d->mAutoCorrectionSettings->maxFindStringLength());

    // TODO verify that pos == block.position() => it's a full line => not a piece of word
    // TODO if not => check if pos -1 is a space => not a piece of word
    // TODO otherwise move cursor until we detect a space
    // TODO otherwise we must not autoconvert it.
    if (pos != block.position()) {
        const QString text = block.text();
        const int currentPos = (pos - block.position());
        if (!text.at(currentPos - 1).isSpace()) {
            // qDebug() << " current Text " << text << " currentPos "<< currentPos << " pos " << pos;
            // qDebug() << "selected text " << text.right(text.length() - currentPos);
            // qDebug() << "  text after " << text.at(currentPos - 1);
            bool foundWord = false;
            const int textLength(text.length());
            for (int i = currentPos; i < textLength; ++i) {
                if (text.at(i).isSpace()) {
                    pos = qMin(cursorPosition, pos + 1 + block.position());
                    foundWord = true;
                    break;
                }
            }
            if (!foundWord) {
                pos = cursorPosition;
            }
        }
    }
    cursor.setPosition(pos);
    cursor.setPosition(cursorPosition, QTextCursor::KeepAnchor);
}

void AutoCorrection::selectPreviousWord(QTextCursor &cursor, int cursorPosition)
{
    cursor.setPosition(cursorPosition);
    QTextBlock block = cursor.block();
    cursor.setPosition(block.position());
    cursorPosition -= block.position();
    QString string = block.text();
    int pos = 0;
    bool space = false;
    QString::Iterator iter = string.begin();
    while (iter != string.end()) {
        if (iter->isSpace()) {
            if (space) {
                // double spaces belong to the previous word
            } else if (pos < cursorPosition) {
                cursor.setPosition(pos + block.position() + 1); // +1 because we don't want to set it on the space itself
            } else {
                space = true;
            }
        } else if (space) {
            break;
        }
        ++pos;
        ++iter;
    }
    cursor.setPosition(pos + block.position(), QTextCursor::KeepAnchor);
}

bool AutoCorrection::autocorrect(bool htmlMode, QTextDocument &document, int &position)
{
    if (d->mAutoCorrectionSettings->isEnabledAutoCorrection()) {
        d->mCursor = QTextCursor(&document);
        d->mCursor.setPosition(position);

        // If we already have a space not necessary to look at other autocorrect feature.
        if (!singleSpaces()) {
            return false;
        }

        int oldPosition = position;
        selectPreviousWord(d->mCursor, position);
        d->mWord = d->mCursor.selectedText();
        if (d->mWord.isEmpty()) {
            return true;
        }
        d->mCursor.beginEditBlock();
        bool done = false;
        if (htmlMode) {
            done = autoFormatURLs();
            if (!done) {
                done = autoBoldUnderline();
                // We replace */- by format => remove cursor position by 2
                if (done) {
                    oldPosition -= 2;
                }
            }
            if (!done) {
                superscriptAppendix();
            }
        }
        if (!done) {
            done = autoFractions();
            // We replace three characters with 1
            if (done) {
                oldPosition -= 2;
            }
        }
        if (!done) {
            uppercaseFirstCharOfSentence();
            fixTwoUppercaseChars();
            capitalizeWeekDays();
            replaceTypographicQuotes();
            if (d->mWord.length() <= 2) {
                addNonBreakingSpace();
            }
        }

        if (d->mCursor.selectedText() != d->mWord) {
            d->mCursor.insertText(d->mWord);
        }
        position = oldPosition;
        if (!done) {
            selectStringOnMaximumSearchString(d->mCursor, position);
            d->mWord = d->mCursor.selectedText();
            if (!d->mWord.isEmpty()) {
                const QStringList lst = AutoCorrectionUtils::wordsFromSentence(d->mWord);
                qCDebug(PIMCOMMONAUTOCORRECTION_AUTOCORRECT_LOG) << " lst " << lst;
                for (const auto &string : lst) {
                    const int diffSize = d->mWord.length() - string.length();
                    d->mWord = string;
                    const int positionEnd(d->mCursor.selectionEnd());
                    d->mCursor.setPosition(d->mCursor.selectionStart() + diffSize);
                    d->mCursor.setPosition(positionEnd, QTextCursor::KeepAnchor);
                    const int newPos = advancedAutocorrect();
                    if (newPos != -1) {
                        if (d->mCursor.selectedText() != d->mWord) {
                            d->mCursor.insertText(d->mWord);
                        }
                        position = newPos;
                        break;
                    }
                }
            }
        }
        d->mCursor.endEditBlock();
    }
    return true;
}

void AutoCorrection::readConfig()
{
    d->mAutoCorrectionSettings->readConfig();
}

void AutoCorrection::writeConfig()
{
    d->mAutoCorrectionSettings->writeConfig();
}

void AutoCorrection::superscriptAppendix()
{
    if (!d->mAutoCorrectionSettings->isSuperScript()) {
        return;
    }

    const QString trimmed = d->mWord.trimmed();
    int startPos = -1;
    int endPos = -1;
    const int trimmedLenght(trimmed.length());

    QHash<QString, QString>::const_iterator i = d->mAutoCorrectionSettings->superScriptEntries().constBegin();
    while (i != d->mAutoCorrectionSettings->superScriptEntries().constEnd()) {
        if (i.key() == trimmed) {
            startPos = d->mCursor.selectionStart() + 1;
            endPos = startPos - 1 + trimmedLenght;
            break;
        } else if (i.key() == QLatin1String("othernb")) {
            const int pos = trimmed.indexOf(i.value());
            if (pos > 0) {
                QString number = trimmed.left(pos);
                QString::ConstIterator constIter = number.constBegin();
                bool found = true;
                // don't apply superscript to 1th, 2th and 3th
                const int numberLength(number.length());
                if (numberLength == 1 && (*constIter == QLatin1Char('1') || *constIter == QLatin1Char('2') || *constIter == QLatin1Char('3'))) {
                    found = false;
                }
                if (found) {
                    while (constIter != number.constEnd()) {
                        if (!constIter->isNumber()) {
                            found = false;
                            break;
                        }
                        ++constIter;
                    }
                }
                if (found && numberLength + i.value().length() == trimmedLenght) {
                    startPos = d->mCursor.selectionStart() + pos;
                    endPos = startPos - pos + trimmedLenght;
                    break;
                }
            }
        }
        ++i;
    }

    if (startPos != -1 && endPos != -1) {
        QTextCursor cursor(d->mCursor);
        cursor.setPosition(startPos);
        cursor.setPosition(endPos, QTextCursor::KeepAnchor);

        QTextCharFormat format;
        format.setVerticalAlignment(QTextCharFormat::AlignSuperScript);
        cursor.mergeCharFormat(format);
    }
}

void AutoCorrection::addNonBreakingSpace()
{
    if (d->mAutoCorrectionSettings->isAddNonBreakingSpace() && d->mAutoCorrectionSettings->isFrenchLanguage()) {
        const QTextBlock block = d->mCursor.block();
        const QString text = block.text();
        const QChar lastChar = text.at(d->mCursor.position() - 1 - block.position());

        if (lastChar == QLatin1Char(':') || lastChar == QLatin1Char(';') || lastChar == QLatin1Char('!') || lastChar == QLatin1Char('?')
            || lastChar == QLatin1Char('%')) {
            const int pos = d->mCursor.position() - 2 - block.position();
            if (pos >= 0) {
                const QChar previousChar = text.at(pos);
                if (previousChar.isSpace()) {
                    QTextCursor cursor(d->mCursor);
                    cursor.setPosition(pos);
                    cursor.setPosition(pos + 1, QTextCursor::KeepAnchor);
                    cursor.deleteChar();
                    d->mCursor.insertText(d->mAutoCorrectionSettings->nonBreakingSpace());
                }
            }
        } else {
            // °C (degrees)
            const int pos = d->mCursor.position() - 2 - block.position();
            if (pos >= 0) {
                const QChar previousChar = text.at(pos);

                if (lastChar == QLatin1Char('C') && previousChar == QChar(0x000B0)) {
                    const int pos = d->mCursor.position() - 3 - block.position();
                    if (pos >= 0) {
                        const QChar previousChar = text.at(pos);
                        if (previousChar.isSpace()) {
                            QTextCursor cursor(d->mCursor);
                            cursor.setPosition(pos);
                            cursor.setPosition(pos + 1, QTextCursor::KeepAnchor);
                            cursor.deleteChar();
                            d->mCursor.insertText(d->mAutoCorrectionSettings->nonBreakingSpace());
                        }
                    }
                }
            }
        }
    }
}

bool AutoCorrection::autoBoldUnderline()
{
    if (!d->mAutoCorrectionSettings->isAutoBoldUnderline()) {
        return false;
    }
    const QString trimmed = d->mWord.trimmed();

    const auto trimmedLength{trimmed.length()};
    if (trimmedLength < 3) {
        return false;
    }

    const QChar trimmedFirstChar(trimmed.at(0));
    const QChar trimmedLastChar(trimmed.at(trimmedLength - 1));
    const bool underline = (trimmedFirstChar == QLatin1Char('_') && trimmedLastChar == QLatin1Char('_'));
    const bool bold = (trimmedFirstChar == QLatin1Char('*') && trimmedLastChar == QLatin1Char('*'));
    const bool strikeOut = (trimmedFirstChar == QLatin1Char('-') && trimmedLastChar == QLatin1Char('-'));
    if (underline || bold || strikeOut) {
        const int startPos = d->mCursor.selectionStart();
        const QString replacement = trimmed.mid(1, trimmedLength - 2);
        bool foundLetterNumber = false;

        QString::ConstIterator constIter = replacement.constBegin();
        while (constIter != replacement.constEnd()) {
            if (constIter->isLetterOrNumber()) {
                foundLetterNumber = true;
                break;
            }
            ++constIter;
        }

        // if no letter/number found, don't apply autocorrection like in OOo 2.x
        if (!foundLetterNumber) {
            return false;
        }
        d->mCursor.setPosition(startPos);
        d->mCursor.setPosition(startPos + trimmedLength, QTextCursor::KeepAnchor);
        d->mCursor.insertText(replacement);
        d->mCursor.setPosition(startPos);
        d->mCursor.setPosition(startPos + replacement.length(), QTextCursor::KeepAnchor);

        QTextCharFormat format;
        format.setFontUnderline(underline ? true : d->mCursor.charFormat().fontUnderline());
        format.setFontWeight(bold ? QFont::Bold : d->mCursor.charFormat().fontWeight());
        format.setFontStrikeOut(strikeOut ? true : d->mCursor.charFormat().fontStrikeOut());
        d->mCursor.mergeCharFormat(format);

        // to avoid the selection being replaced by d->mWord
        d->mWord = d->mCursor.selectedText();
        return true;
    } else {
        return false;
    }

    return true;
}

QColor AutoCorrection::linkColor()
{
    if (d->mLinkColor.isValid()) {
        return d->mLinkColor;
    }
    d->mLinkColor = KColorScheme(QPalette::Active, KColorScheme::View).foreground(KColorScheme::LinkText).color();
    return d->mLinkColor;
}

AutoCorrectionSettings *AutoCorrection::autoCorrectionSettings() const
{
    return d->mAutoCorrectionSettings;
}

void AutoCorrection::setAutoCorrectionSettings(AutoCorrectionSettings *newAutoCorrectionSettings)
{
    if (d->mAutoCorrectionSettings != newAutoCorrectionSettings) {
        delete d->mAutoCorrectionSettings;
    }
    d->mAutoCorrectionSettings = newAutoCorrectionSettings;
}

bool AutoCorrection::autoFormatURLs()
{
    if (!d->mAutoCorrectionSettings->isAutoFormatUrl()) {
        return false;
    }

    const QString link = autoDetectURL(d->mWord);
    if (link.isNull()) {
        return false;
    }

    const QString trimmed = d->mWord.trimmed();
    const int startPos = d->mCursor.selectionStart();
    d->mCursor.setPosition(startPos);
    d->mCursor.setPosition(startPos + trimmed.length(), QTextCursor::KeepAnchor);

    QTextCharFormat format;
    format.setAnchorHref(link);
    format.setFontItalic(true);
    format.setAnchor(true);
    format.setUnderlineStyle(QTextCharFormat::SingleUnderline);
    format.setUnderlineColor(linkColor());
    format.setForeground(linkColor());
    d->mCursor.mergeCharFormat(format);

    d->mWord = d->mCursor.selectedText();
    return true;
}

QString AutoCorrection::autoDetectURL(const QString &_word) const
{
    QString word = _word;
    /* this method is ported from lib/kotext/KoAutoFormat.cpp KoAutoFormat::doAutoDetectUrl
     * from Calligra 1.x branch */
    // qCDebug(PIMCOMMON_LOG) << "link:" << word;

    // we start by iterating through a list of schemes, and if no match is found,
    // we proceed to 3 special cases

    // list of the schemes, starting with http:// as most probable
    const QStringList schemes = QStringList() << QStringLiteral("http://") << QStringLiteral("https://") << QStringLiteral("mailto:/")
                                              << QStringLiteral("ftp://") << QStringLiteral("file://") << QStringLiteral("git://") << QStringLiteral("sftp://")
                                              << QStringLiteral("magnet:?") << QStringLiteral("smb://") << QStringLiteral("nfs://") << QStringLiteral("fish://")
                                              << QStringLiteral("ssh://") << QStringLiteral("telnet://") << QStringLiteral("irc://") << QStringLiteral("sip:")
                                              << QStringLiteral("news:") << QStringLiteral("gopher://") << QStringLiteral("nntp://") << QStringLiteral("geo:")
                                              << QStringLiteral("udp://") << QStringLiteral("rsync://") << QStringLiteral("dns://");

    enum LinkType {
        UNCLASSIFIED,
        SCHEME,
        MAILTO,
        WWW,
        FTP,
    };
    LinkType linkType = UNCLASSIFIED;
    int pos = 0;
    int contentPos = 0;

    // TODO: ideally there would be proper pattern matching,
    // instead of just searching for some key string, like done with indexOf.
    // This should reduce the amount of possible mismatches
    for (const QString &scheme : schemes) {
        pos = word.indexOf(scheme);
        if (pos != -1) {
            linkType = SCHEME;
            contentPos = pos + scheme.length();
            break; // break as soon as you get a match
        }
    }

    if (linkType == UNCLASSIFIED) {
        pos = word.indexOf(QLatin1String("www."), 0, Qt::CaseInsensitive);
        if (pos != -1 && word.indexOf(QLatin1Char('.'), pos + 4) != -1) {
            linkType = WWW;
            contentPos = pos + 4;
        }
    }
    if (linkType == UNCLASSIFIED) {
        pos = word.indexOf(QLatin1String("ftp."), 0, Qt::CaseInsensitive);
        if (pos != -1 && word.indexOf(QLatin1Char('.'), pos + 4) != -1) {
            linkType = FTP;
            contentPos = pos + 4;
        }
    }
    if (linkType == UNCLASSIFIED) {
        const int separatorPos = word.lastIndexOf(QLatin1Char('@'));
        if (separatorPos != -1) {
            pos = separatorPos - 1;
            QChar c;
            while (pos >= 0) {
                c = word.at(pos);
                if ((c.isPunct() && c != QLatin1Char('.') && c != QLatin1Char('_')) || (c == QLatin1Char('@'))) {
                    pos = -2;
                    break;
                } else {
                    --pos;
                }
            }
            if (pos == -1) { // a valid address
                ++pos;
                contentPos = separatorPos + 1;
                linkType = MAILTO;
            }
        }
    }

    if (linkType != UNCLASSIFIED) {
        // A URL inside e.g. quotes (like "http://www.calligra.org" with the quotes)
        // shouldn't include the quote in the URL.
        int lastPos = word.length() - 1;
        while (!word.at(lastPos).isLetter() && !word.at(lastPos).isDigit() && word.at(lastPos) != QLatin1Char('/')) {
            --lastPos;
        }
        // sanity check: was there no real content behind the key string?
        if (lastPos < contentPos) {
            return QString();
        }
        word.truncate(lastPos + 1);
        word.remove(0, pos);
        switch (linkType) {
        case MAILTO:
            word.prepend(QLatin1String("mailto:"));
            break;
        case WWW:
            word.prepend(QLatin1String("http://"));
            break;
        case FTP:
            word.prepend(QLatin1String("ftps://"));
            break;
        case SCHEME:
        case UNCLASSIFIED:
            break;
        }
        return word;
    }
    return {};
}

void AutoCorrection::fixTwoUppercaseChars()
{
    if (!d->mAutoCorrectionSettings->isFixTwoUppercaseChars()) {
        return;
    }
    if (d->mWord.length() <= 2) {
        return;
    }

    if (d->mAutoCorrectionSettings->twoUpperLetterExceptions().contains(d->mWord.trimmed())) {
        return;
    }

    const QChar firstChar = d->mWord.at(0);
    const QChar secondChar = d->mWord.at(1);

    if (secondChar.isUpper() && firstChar.isUpper()) {
        const QChar thirdChar = d->mWord.at(2);

        if (thirdChar.isLower()) {
            d->mWord.replace(1, 1, secondChar.toLower());
        }
    }
}

// Return true if we can add space
bool AutoCorrection::singleSpaces() const
{
    if (!d->mAutoCorrectionSettings->isSingleSpaces()) {
        return true;
    }
    if (!d->mCursor.atBlockStart()) {
        // then when the prev char is also a space, don't insert one.
        const QTextBlock block = d->mCursor.block();
        const QString text = block.text();
        if (text.at(d->mCursor.position() - 1 - block.position()) == QLatin1Char(' ')) {
            return false;
        }
    }
    return true;
}

void AutoCorrection::capitalizeWeekDays()
{
    if (!d->mAutoCorrectionSettings->isCapitalizeWeekDays()) {
        return;
    }

    const QString trimmed = d->mWord.trimmed();
    for (const QString &name : std::as_const(d->mCacheNameOfDays)) {
        if (trimmed == name) {
            const int pos = d->mWord.indexOf(name);
            d->mWord.replace(pos, 1, name.at(0).toUpper());
            return;
        }
    }
}

bool AutoCorrection::excludeToUppercase(const QString &word) const
{
    if (word.startsWith(QLatin1String("http://")) || word.startsWith(QLatin1String("www.")) || word.startsWith(QLatin1String("mailto:"))
        || word.startsWith(QLatin1String("ftp://")) || word.startsWith(QLatin1String("https://")) || word.startsWith(QLatin1String("ftps://"))) {
        return true;
    }
    return false;
}

void AutoCorrection::uppercaseFirstCharOfSentence()
{
    if (!d->mAutoCorrectionSettings->isUppercaseFirstCharOfSentence()) {
        return;
    }

    int startPos = d->mCursor.selectionStart();
    QTextBlock block = d->mCursor.block();

    d->mCursor.setPosition(block.position());
    d->mCursor.setPosition(startPos, QTextCursor::KeepAnchor);

    int position = d->mCursor.selectionEnd();

    const QString text = d->mCursor.selectedText();

    if (text.isEmpty()) { // start of a paragraph
        if (!excludeToUppercase(d->mWord)) {
            d->mWord.replace(0, 1, d->mWord.at(0).toUpper());
        }
    } else {
        QString::ConstIterator constIter = text.constEnd();
        --constIter;

        while (constIter != text.constBegin()) {
            while (constIter != text.begin() && constIter->isSpace()) {
                --constIter;
                --position;
            }

            if (constIter != text.constBegin() && (*constIter == QLatin1Char('.') || *constIter == QLatin1Char('!') || *constIter == QLatin1Char('?'))) {
                constIter--;
                while (constIter != text.constBegin() && !(constIter->isLetter())) {
                    --position;
                    --constIter;
                }
                selectPreviousWord(d->mCursor, --position);
                const QString prevWord = d->mCursor.selectedText();

                // search for exception
                if (d->mAutoCorrectionSettings->upperCaseExceptions().contains(prevWord.trimmed())) {
                    break;
                }
                if (excludeToUppercase(d->mWord)) {
                    break;
                }

                d->mWord.replace(0, 1, d->mWord.at(0).toUpper());
                break;
            } else {
                break;
            }
        }
    }

    d->mCursor.setPosition(startPos);
    d->mCursor.setPosition(startPos + d->mWord.length(), QTextCursor::KeepAnchor);
}

bool AutoCorrection::autoFractions() const
{
    if (!d->mAutoCorrectionSettings->isAutoFractions()) {
        return false;
    }

    const QString trimmed = d->mWord.trimmed();
    const auto trimmedLength{trimmed.length()};
    if (trimmedLength > 3) {
        const QChar x = trimmed.at(3);
        const uchar xunicode = x.unicode();
        if (!(xunicode == '.' || xunicode == ',' || xunicode == '?' || xunicode == '!' || xunicode == ':' || xunicode == ';')) {
            return false;
        }
    } else if (trimmedLength < 3) {
        return false;
    }

    if (trimmed.startsWith(QLatin1String("1/2"))) {
        d->mWord.replace(0, 3, QStringLiteral("½"));
    } else if (trimmed.startsWith(QLatin1String("1/4"))) {
        d->mWord.replace(0, 3, QStringLiteral("¼"));
    } else if (trimmed.startsWith(QLatin1String("3/4"))) {
        d->mWord.replace(0, 3, QStringLiteral("¾"));
    } else {
        return false;
    }

    return true;
}

int AutoCorrection::advancedAutocorrect()
{
    if (!d->mAutoCorrectionSettings->isAdvancedAutocorrect()) {
        return -1;
    }
    if (d->mAutoCorrectionSettings->autocorrectEntries().isEmpty()) {
        return -1;
    }
    const QString trimmedWord = d->mWord.trimmed();
    if (trimmedWord.isEmpty()) {
        return -1;
    }
    QString actualWord = trimmedWord;

    const int actualWordLength(actualWord.length());
    if (actualWordLength < d->mAutoCorrectionSettings->minFindStringLength()) {
        return -1;
    }
    if (actualWordLength > d->mAutoCorrectionSettings->maxFindStringLength()) {
        return -1;
    }
    const int startPos = d->mCursor.selectionStart();
    qCDebug(PIMCOMMONAUTOCORRECTION_AUTOCORRECT_LOG) << "d->mCursor  " << d->mCursor.selectedText() << " startPos " << startPos;
    const int length = d->mWord.length();
    // If the last char is punctuation, drop it for now
    bool hasPunctuation = false;
    const QChar lastChar = actualWord.at(actualWord.length() - 1);
    const ushort charUnicode = lastChar.unicode();
    if (charUnicode == '.' || charUnicode == ',' || charUnicode == '?' || charUnicode == '!' || charUnicode == ';') {
        hasPunctuation = true;
        actualWord.chop(1);
    } else if (charUnicode == ':' && actualWord.at(0).unicode() != ':') {
        hasPunctuation = true;
        actualWord.chop(1);
    }
    QString actualWordWithFirstUpperCase = actualWord;
    actualWordWithFirstUpperCase[0] = actualWordWithFirstUpperCase[0].toUpper();
    QHashIterator<QString, QString> i(d->mAutoCorrectionSettings->autocorrectEntries());
    while (i.hasNext()) {
        i.next();
        const auto key = i.key();
        const auto keyLength{key.length()};
        if (hasPunctuation) {
            // We remove 1 element when we have punctuation
            if (keyLength != actualWordLength - 1) {
                continue;
            }
        } else {
            if (keyLength != actualWordLength) {
                continue;
            }
        }
        qCDebug(PIMCOMMONAUTOCORRECTION_AUTOCORRECT_LOG) << " i.key() " << key << "actual" << actualWord;
        if (actualWord.endsWith(key) || actualWord.endsWith(key, Qt::CaseInsensitive) || actualWordWithFirstUpperCase.endsWith(key)) {
            int pos = d->mWord.lastIndexOf(key);
            qCDebug(PIMCOMMONAUTOCORRECTION_AUTOCORRECT_LOG) << " pos 1 " << pos << " d->mWord " << d->mWord;
            if (pos == -1) {
                pos = actualWord.toLower().lastIndexOf(key);
                qCDebug(PIMCOMMONAUTOCORRECTION_AUTOCORRECT_LOG) << " pos 2 " << pos;
                if (pos == -1) {
                    pos = actualWordWithFirstUpperCase.lastIndexOf(key);
                    qCDebug(PIMCOMMONAUTOCORRECTION_AUTOCORRECT_LOG) << " pos 3 " << pos;
                    if (pos == -1) {
                        continue;
                    }
                }
            }
            QString replacement = i.value();

            // qDebug() << " actualWord " << actualWord << " pos " << pos << " actualWord.size" << actualWord.length() << "actualWordWithFirstUpperCase "
            // <<actualWordWithFirstUpperCase; qDebug() << " d->mWord " << d->mWord << " i.key() " << i.key() << "replacement " << replacement; Keep capitalized
            // words capitalized. (Necessary to make sure the first letters match???)
            const QChar actualWordFirstChar = d->mWord.at(pos);
            qCDebug(PIMCOMMONAUTOCORRECTION_AUTOCORRECT_LOG) << " actualWordFirstChar " << actualWordFirstChar;

            const QChar replacementFirstChar = replacement[0];
            if (actualWordFirstChar.isUpper() && replacementFirstChar.isLower()) {
                replacement[0] = replacementFirstChar.toUpper();
            } else if (actualWordFirstChar.isLower() && replacementFirstChar.isUpper()) {
                replacement[0] = replacementFirstChar.toLower();
            }

            // If a punctuation mark was on the end originally, add it back on
            if (hasPunctuation) {
                replacement.append(lastChar);
            }

            d->mWord.replace(pos, pos + trimmedWord.length(), replacement);

            // We do replacement here, since the length of new word might be different from length of
            // the old world. Length difference might affect other type of autocorrection
            d->mCursor.setPosition(startPos);
            d->mCursor.setPosition(startPos + length, QTextCursor::KeepAnchor);
            d->mCursor.insertText(d->mWord);
            qCDebug(PIMCOMMONAUTOCORRECTION_AUTOCORRECT_LOG) << " insert text " << d->mWord << " startPos " << startPos;
            d->mCursor.setPosition(startPos); // also restore the selection
            const int newPosition = startPos + d->mWord.length();
            d->mCursor.setPosition(newPosition, QTextCursor::KeepAnchor);
            return newPosition;
        }
    }
    return -1;
}

void AutoCorrection::replaceTypographicQuotes()
{
    // TODO add french quotes support.
    /* this method is ported from lib/kotext/KoAutoFormat.cpp KoAutoFormat::doTypographicQuotes
     * from Calligra 1.x branch */

    if (!(d->mAutoCorrectionSettings->isReplaceDoubleQuotes() && d->mWord.contains(QLatin1Char('"')))
        && !(d->mAutoCorrectionSettings->isReplaceSingleQuotes() && d->mWord.contains(QLatin1Char('\'')))) {
        return;
    }

    const bool addNonBreakingSpace = (d->mAutoCorrectionSettings->isFrenchLanguage() && d->mAutoCorrectionSettings->isAddNonBreakingSpace());

    // Need to determine if we want a starting or ending quote.
    // we use a starting quote in three cases:
    //  1. if the previous character is a space
    //  2. if the previous character is some kind of opening punctuation (e.g., "(", "[", or "{")
    //     a. and the character before that is not an opening quote (so that we get quotations of single characters
    //        right)
    //  3. if the previous character is an opening quote (so that we get nested quotations right)
    //     a. and the character before that is not an opening quote (so that we get quotations of single characters
    //         right)
    //     b. and the previous quote of a different kind (so that we get empty quotations right)

    bool ending = true;
    for (int i = d->mWord.length(); i > 1; --i) {
        const QChar c = d->mWord.at(i - 1);
        if (c == QLatin1Char('"') || c == QLatin1Char('\'')) {
            const bool doubleQuotes = (c == QLatin1Char('"'));
            if (i > 2) {
                QChar::Category c1 = d->mWord.at(i - 1).category();

                // case 1 and 2
                if (c1 == QChar::Separator_Space || c1 == QChar::Separator_Line || c1 == QChar::Separator_Paragraph || c1 == QChar::Punctuation_Open
                    || c1 == QChar::Other_Control) {
                    ending = false;
                }

                // case 3
                if (c1 == QChar::Punctuation_InitialQuote) {
                    QChar openingQuote;

                    if (doubleQuotes) {
                        if (d->mAutoCorrectionSettings->isReplaceDoubleQuotesByFrenchQuotes()) {
                            openingQuote = d->mAutoCorrectionSettings->doubleFrenchQuotes().begin;
                        } else {
                            openingQuote = d->mAutoCorrectionSettings->typographicDoubleQuotes().begin;
                        }
                    } else {
                        openingQuote = d->mAutoCorrectionSettings->typographicSingleQuotes().begin;
                    }

                    // case 3b
                    if (d->mWord.at(i - 1) != openingQuote) {
                        ending = false;
                    }
                }
            }
            // case 2a and 3a
            if (i > 3 && !ending) {
                const QChar::Category c2 = (d->mWord.at(i - 2)).category();
                ending = (c2 == QChar::Punctuation_InitialQuote);
            }

            if (doubleQuotes && d->mAutoCorrectionSettings->isReplaceDoubleQuotes()) {
                if (ending) {
                    const QChar endQuote = d->mAutoCorrectionSettings->isReplaceDoubleQuotesByFrenchQuotes()
                        ? d->mAutoCorrectionSettings->doubleFrenchQuotes().end
                        : d->mAutoCorrectionSettings->typographicDoubleQuotes().end;
                    if (addNonBreakingSpace) {
                        d->mWord.replace(i - 1, 2, QString(d->mAutoCorrectionSettings->nonBreakingSpace() + endQuote));
                    } else {
                        d->mWord[i - 1] = endQuote;
                    }
                } else {
                    const QChar beginQuote = d->mAutoCorrectionSettings->isReplaceDoubleQuotesByFrenchQuotes()
                        ? d->mAutoCorrectionSettings->doubleFrenchQuotes().begin
                        : d->mAutoCorrectionSettings->typographicDoubleQuotes().begin;
                    if (addNonBreakingSpace) {
                        d->mWord.replace(i - 1, 2, QString(d->mAutoCorrectionSettings->nonBreakingSpace() + beginQuote));
                    } else {
                        d->mWord[i - 1] = beginQuote;
                    }
                }
            } else if (d->mAutoCorrectionSettings->isReplaceSingleQuotes()) {
                if (ending) {
                    if (addNonBreakingSpace) {
                        d->mWord.replace(i - 1,
                                         2,
                                         QString(d->mAutoCorrectionSettings->nonBreakingSpace() + d->mAutoCorrectionSettings->typographicSingleQuotes().end));
                    } else {
                        d->mWord[i - 1] = d->mAutoCorrectionSettings->typographicSingleQuotes().end;
                    }
                } else {
                    if (addNonBreakingSpace) {
                        d->mWord.replace(i - 1,
                                         2,
                                         QString(d->mAutoCorrectionSettings->nonBreakingSpace() + d->mAutoCorrectionSettings->typographicSingleQuotes().begin));
                    } else {
                        d->mWord[i - 1] = d->mAutoCorrectionSettings->typographicSingleQuotes().begin;
                    }
                }
            }
        }
    }

    // first character
    if (d->mWord.at(0) == QLatin1Char('"') && d->mAutoCorrectionSettings->isReplaceDoubleQuotes()) {
        const QChar beginQuote = d->mAutoCorrectionSettings->isReplaceDoubleQuotesByFrenchQuotes()
            ? d->mAutoCorrectionSettings->doubleFrenchQuotes().begin
            : d->mAutoCorrectionSettings->typographicDoubleQuotes().begin;
        d->mWord[0] = beginQuote;
        if (addNonBreakingSpace) {
            d->mWord.insert(1, d->mAutoCorrectionSettings->nonBreakingSpace());
        }
    } else if (d->mWord.at(0) == QLatin1Char('\'') && d->mAutoCorrectionSettings->isReplaceSingleQuotes()) {
        d->mWord[0] = d->mAutoCorrectionSettings->typographicSingleQuotes().begin;
        if (addNonBreakingSpace) {
            d->mWord.insert(1, d->mAutoCorrectionSettings->nonBreakingSpace());
        }
    }
}

void AutoCorrection::loadGlobalFileName(const QString &fname)
{
    d->mAutoCorrectionSettings->loadGlobalFileName(fname);
}

void AutoCorrection::writeAutoCorrectionXmlFile(const QString &filename)
{
    d->mAutoCorrectionSettings->writeAutoCorrectionFile(filename);
}
