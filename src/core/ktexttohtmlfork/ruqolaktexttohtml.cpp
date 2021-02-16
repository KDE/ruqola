/*
    SPDX-FileCopyrightText: 2002 Dave Corrie <kde@davecorrie.com>
    SPDX-FileCopyrightText: 2014 Daniel Vrátil <dvratil@redhat.com>

    SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "ruqolaktexttohtml.h"
#include "ruqolaktexttohtml_p.h"

#include <QCoreApplication>
#include <QFile>
#include <QPluginLoader>
#include <QRegularExpression>
#include <QStringList>

#include <limits.h>

KTextToHTMLHelper::KTextToHTMLHelper(const QString &plainText, int pos, int maxUrlLen, int maxAddressLen)
    : mText(plainText)
    , mMaxUrlLen(maxUrlLen)
    , mMaxAddressLen(maxAddressLen)
    , mPos(pos)
{
}

QString KTextToHTMLHelper::getEmailAddress()
{
    QString address;

    if (mPos < mText.length() && mText.at(mPos) == QLatin1Char('@')) {
        // the following characters are allowed in a dot-atom (RFC 2822):
        // a-z A-Z 0-9 . ! # $ % & ' * + - / = ? ^ _ ` { | } ~
        const QString allowedSpecialChars = QStringLiteral(".!#$%&'*+-/=?^_`{|}~");

        // determine the local part of the email address
        int start = mPos - 1;
        while (start >= 0 && mText.at(start).unicode() < 128
               && (mText.at(start).isLetterOrNumber() || mText.at(start) == QLatin1Char('@') // allow @ to find invalid email addresses
                   || allowedSpecialChars.indexOf(mText.at(start)) != -1)) {
            if (mText.at(start) == QLatin1Char('@')) {
                return QString(); // local part contains '@' -> no email address
            }
            --start;
        }
        ++start;
        // we assume that an email address starts with a letter or a digit
        while ((start < mPos) && !mText.at(start).isLetterOrNumber()) {
            ++start;
        }
        if (start == mPos) {
            return QString(); // local part is empty -> no email address
        }

        // determine the domain part of the email address
        int dotPos = INT_MAX;
        int end = mPos + 1;
        while (end < mText.length()
               && (mText.at(end).isLetterOrNumber() || mText.at(end) == QLatin1Char('@') // allow @ to find invalid email addresses
                   || mText.at(end) == QLatin1Char('.') || mText.at(end) == QLatin1Char('-'))) {
            if (mText.at(end) == QLatin1Char('@')) {
                return QString(); // domain part contains '@' -> no email address
            }
            if (mText.at(end) == QLatin1Char('.')) {
                dotPos = qMin(dotPos, end); // remember index of first dot in domain
            }
            ++end;
        }
        // we assume that an email address ends with a letter or a digit
        while ((end > mPos) && !mText.at(end - 1).isLetterOrNumber()) {
            --end;
        }
        if (end == mPos) {
            return QString(); // domain part is empty -> no email address
        }
        if (dotPos >= end) {
            return QString(); // domain part doesn't contain a dot
        }

        if (end - start > mMaxAddressLen) {
            return QString(); // too long -> most likely no email address
        }
        address = mText.mid(start, end - start);

        mPos = end - 1;
    }
    return address;
}

QString KTextToHTMLHelper::getPhoneNumber()
{
    if (!mText.at(mPos).isDigit() && mText.at(mPos) != QLatin1Char('+')) {
        return {};
    }

    const QString allowedBeginSeparators = QStringLiteral(" \r\t\n:");
    if (mPos > 0 && !allowedBeginSeparators.contains(mText.at(mPos - 1))) {
        return {};
    }

    // this isn't 100% accurate, we filter stuff below that is too hard to capture with a regexp
    static const QRegularExpression telPattern(QStringLiteral(R"([+0](( |( ?[/-] ?)?)\(?\d+\)?+){6,30})"));
    const auto match = telPattern.match(mText, mPos, QRegularExpression::NormalMatch, QRegularExpression::AnchoredMatchOption);
    if (match.hasMatch()) {
        auto m = match.captured();
        // check for maximum number of digits (15), see https://en.wikipedia.org/wiki/Telephone_numbering_plan
        if (std::count_if(m.begin(),
                          m.end(),
                          [](const QChar &c) {
                              return c.isDigit();
                          })
            > 15) {
            return {};
        }
        // only one / is allowed, otherwise we trigger on dates
        if (std::count(m.begin(), m.end(), QLatin1Char('/')) > 1) {
            return {};
        }

        // parenthesis need to be balanced, and must not be nested
        int openIdx = -1;
        for (int i = 0; i < m.size(); ++i) {
            if ((m[i] == QLatin1Char('(') && openIdx >= 0) || (m[i] == QLatin1Char(')') && openIdx < 0)) {
                return {};
            }
            if (m[i] == QLatin1Char('(')) {
                openIdx = i;
            } else if (m[i] == QLatin1Char(')')) {
                openIdx = -1;
            }
        }
        if (openIdx > 0) {
            m = m.leftRef(openIdx - 1).trimmed().toString();
        }

        // check if there's a plausible separator at the end
        const QString allowedEndSeparators = QStringLiteral(" \r\t\n,.");
        const auto l = m.size();
        if (mText.size() > mPos + l && !allowedEndSeparators.contains(mText.at(mPos + l))) {
            return {};
        }

        mPos += l - 1;
        return m;
    }
    return {};
}

static QString normalizePhoneNumber(const QString &str)
{
    QString res;
    res.reserve(str.size());
    for (const auto c : str) {
        if (c.isDigit() || c == QLatin1Char('+')) {
            res.push_back(c);
        }
    }
    return res;
}

// The following characters are allowed in a dot-atom (RFC 2822):
// a-z A-Z 0-9 . ! # $ % & ' * + - / = ? ^ _ ` { | } ~
static const char s_allowedSpecialChars[] = ".!#$%&'*+-/=?^_`{|}~";

bool KTextToHTMLHelper::atUrl() const
{
    // The character directly before the URL must not be a letter, a number or
    // any other character allowed in a dot-atom (RFC 2822).
    if (mPos > 0) {
        const auto chBefore = mText.at(mPos - 1);
        if (chBefore.isLetterOrNumber() || QString::fromLatin1(s_allowedSpecialChars).contains(chBefore)) {
            return false;
        }
    }

    const auto segment = mText.midRef(mPos);
    return segment.startsWith(QLatin1String("http://")) || segment.startsWith(QLatin1String("https://")) || segment.startsWith(QLatin1String("vnc://"))
        || segment.startsWith(QLatin1String("fish://")) || segment.startsWith(QLatin1String("ftp://")) || segment.startsWith(QLatin1String("ftps://"))
        || segment.startsWith(QLatin1String("sftp://")) || segment.startsWith(QLatin1String("smb://")) || segment.startsWith(QLatin1String("mailto:"))
        || segment.startsWith(QLatin1String("www.")) || segment.startsWith(QLatin1String("ftp.")) || segment.startsWith(QLatin1String("file://"))
        || segment.startsWith(QLatin1String("news:")) || segment.startsWith(QLatin1String("tel:")) || segment.startsWith(QLatin1String("xmpp:"));
}

bool KTextToHTMLHelper::isEmptyUrl(const QString &url) const
{
    return url.isEmpty() || url == QLatin1String("http://") || url == QLatin1String("https://") || url == QLatin1String("fish://")
        || url == QLatin1String("ftp://") || url == QLatin1String("ftps://") || url == QLatin1String("sftp://") || url == QLatin1String("smb://")
        || url == QLatin1String("vnc://") || url == QLatin1String("mailto") || url == QLatin1String("mailto:") || url == QLatin1String("www")
        || url == QLatin1String("ftp") || url == QLatin1String("news:") || url == QLatin1String("news://") || url == QLatin1String("tel")
        || url == QLatin1String("tel:") || url == QLatin1String("xmpp:");
}

QString KTextToHTMLHelper::getUrl(bool *badurl)
{
    QString url;
    if (atUrl()) {
        // NOTE: see http://tools.ietf.org/html/rfc3986#appendix-A and especially appendix-C
        // Appendix-C mainly says, that when extracting URLs from plain text, line breaks shall
        // be allowed and should be ignored when the URI is extracted.

        // This implementation follows this recommendation and
        // allows the URL to be enclosed within different kind of brackets/quotes
        // If an URL is enclosed, whitespace characters are allowed and removed, otherwise
        // the URL ends with the first whitespace
        // Also, if the URL is enclosed in brackets, the URL itself is not allowed
        // to contain the closing bracket, as this would be detected as the end of the URL

        QChar beforeUrl, afterUrl;

        // detect if the url has been surrounded by brackets or quotes
        if (mPos > 0) {
            beforeUrl = mText.at(mPos - 1);

            /*if ( beforeUrl == '(' ) {
              afterUrl = ')';
            } else */
            if (beforeUrl == QLatin1Char('[')) {
                afterUrl = QLatin1Char(']');
            } else if (beforeUrl == QLatin1Char('<')) {
                afterUrl = QLatin1Char('>');
            } else if (beforeUrl == QLatin1Char('>')) { // for e.g. <link>http://.....</link>
                afterUrl = QLatin1Char('<');
            } else if (beforeUrl == QLatin1Char('"')) {
                afterUrl = QLatin1Char('"');
            }
        }
        url.reserve(mMaxUrlLen); // avoid allocs
        int start = mPos;
        bool previousCharIsSpace = false;
        bool previousCharIsADoubleQuote = false;
        bool previousIsAnAnchor = false;
        while ((mPos < mText.length()) && (mText.at(mPos).isPrint() || mText.at(mPos).isSpace())
               && ((afterUrl.isNull() && !mText.at(mPos).isSpace()) || (!afterUrl.isNull() && mText.at(mPos) != afterUrl))) {
            if (!previousCharIsSpace && (mText.at(mPos) == QLatin1Char('<')) && ((mPos + 1) < mText.length())) {
                // Fix Bug #346132: allow "http://www.foo.bar<http://foo.bar/>"
                // < inside a URL is not allowed, however there is a test which
                // checks that "http://some<Host>/path" should be allowed
                // Therefore: check if what follows is another URL and if so, stop here
                mPos++;
                if (atUrl()) {
                    mPos--;
                    break;
                }
                mPos--;
            }
            if (!previousCharIsSpace && (mText.at(mPos) == QLatin1Char(' ')) && ((mPos + 1) < mText.length())) {
                // Fix kmail bug: allow "http://www.foo.bar http://foo.bar/"
                // Therefore: check if what follows is another URL and if so, stop here
                mPos++;
                if (atUrl()) {
                    mPos--;
                    break;
                }
                mPos--;
            }
            if (mText.at(mPos).isSpace()) {
                previousCharIsSpace = true;
            } else if (!previousIsAnAnchor && mText.at(mPos) == QLatin1Char('[')) {
                break;
            } else if (!previousIsAnAnchor && mText.at(mPos) == QLatin1Char(']')) {
                break;
            } else { // skip whitespace
                if (previousCharIsSpace && mText.at(mPos) == QLatin1Char('<')) {
                    url.append(QLatin1Char(' '));
                    break;
                }
                previousCharIsSpace = false;
                if (mText.at(mPos) == QLatin1Char('>') && previousCharIsADoubleQuote) {
                    // it's an invalid url
                    if (badurl) {
                        *badurl = true;
                    }
                    return QString();
                }
                if (mText.at(mPos) == QLatin1Char('"')) {
                    previousCharIsADoubleQuote = true;
                } else {
                    previousCharIsADoubleQuote = false;
                }
                if (mText.at(mPos) == QLatin1Char('#')) {
                    previousIsAnAnchor = true;
                }
                url.append(mText.at(mPos));
                if (url.length() > mMaxUrlLen) {
                    break;
                }
            }

            ++mPos;
        }

        if (isEmptyUrl(url) || (url.length() > mMaxUrlLen)) {
            mPos = start;
            url.clear();
            return url;
        } else {
            --mPos;
        }
    }

    // HACK: This is actually against the RFC. However, most people don't properly escape the URL in
    //       their text with "" or <>. That leads to people writing an url, followed immediately by
    //       a dot to finish the sentence. That would lead the parser to include the dot in the url,
    //       even though that is not wanted. So work around that here.
    //       Most real-life URLs hopefully don't end with dots or commas.
    const QString wordBoundaries = QStringLiteral(".,:!?)>");
    if (url.length() > 1) {
        do {
            if (wordBoundaries.contains(url.at(url.length() - 1))) {
                url.chop(1);
                --mPos;
            } else {
                break;
            }
        } while (url.length() > 1);
    }
    return url;
}

QString KTextToHTMLHelper::highlightedText()
{
    // formating symbols must be prepended with a whitespace
    if ((mPos > 0) && !mText.at(mPos - 1).isSpace()) {
        return QString();
    }

    const QChar ch = mText.at(mPos);
    if (ch != QLatin1Char('~') && ch != QLatin1Char('*') && ch != QLatin1Char('_')) {
        return QString();
    }

    QRegularExpression re(QStringLiteral("\\%1([^\\s|^\\%1].*[^\\s|^\\%1])\\%1").arg(ch));
    re.setPatternOptions(QRegularExpression::InvertedGreedinessOption);
    const auto match = re.match(mText, mPos, QRegularExpression::NormalMatch, QRegularExpression::AnchoredMatchOption);

    if (match.hasMatch()) {
        if (match.capturedStart() == mPos) {
            int length = match.capturedLength();
            // there must be a whitespace after the closing formating symbol
            if (mPos + length < mText.length() && !mText.at(mPos + length).isSpace()) {
                return QString();
            }
            mPos += length - 1;
            switch (ch.toLatin1()) {
            case '*':
                return QLatin1String("<b>") + match.capturedRef(1) + QLatin1String("</b>");
            case '_':
                return QLatin1String("<i>") + match.capturedRef(1) + QLatin1String("</i>");
            case '~':
                return QLatin1String("<s>") + match.capturedRef(1) + QLatin1String("</s>");
            }
        }
    }
    return QString();
}

QString RuqolaKTextToHTML::convertToHtml(const QString &plainText, const RuqolaKTextToHTML::Options &flags, int maxUrlLen, int maxAddressLen)
{
    KTextToHTMLHelper helper(plainText, 0, maxUrlLen, maxAddressLen);

    QString str;
    QString result(static_cast<QChar *>(nullptr), helper.mText.length() * 2);
    QChar ch;
    int x;
    bool startOfLine = true;

    for (helper.mPos = 0, x = 0; helper.mPos < helper.mText.length(); ++helper.mPos, ++x) {
        ch = helper.mText.at(helper.mPos);
        if (flags & PreserveSpaces) {
            if (ch == QLatin1Char(' ')) {
                if (helper.mPos + 1 < helper.mText.length()) {
                    if (helper.mText.at(helper.mPos + 1) != QLatin1Char(' ')) {
                        // A single space, make it breaking if not at the start or end of the line
                        const bool endOfLine = helper.mText.at(helper.mPos + 1) == QLatin1Char('\n');
                        if (!startOfLine && !endOfLine) {
                            result += QLatin1Char(' ');
                        } else {
                            result += QLatin1String("&nbsp;");
                        }
                    } else {
                        // Whitespace of more than one space, make it all non-breaking
                        while (helper.mPos < helper.mText.length() && helper.mText.at(helper.mPos) == QLatin1Char(' ')) {
                            result += QLatin1String("&nbsp;");
                            ++helper.mPos;
                            ++x;
                        }

                        // We incremented once to often, undo that
                        --helper.mPos;
                        --x;
                    }
                } else {
                    // Last space in the text, it is non-breaking
                    result += QLatin1String("&nbsp;");
                }

                if (startOfLine) {
                    startOfLine = false;
                }
                continue;
            } else if (ch == QLatin1Char('\t')) {
                do {
                    result += QLatin1String("&nbsp;");
                    ++x;
                } while ((x & 7) != 0);
                --x;
                startOfLine = false;
                continue;
            }
        }
        if (ch == QLatin1Char('\n')) {
            result += QLatin1String("<br />\n"); // Keep the \n, so apps can figure out the quoting levels correctly.
            startOfLine = true;
            x = -1;
            continue;
        }

        startOfLine = false;
        if (ch == QLatin1Char('&')) {
            result += QLatin1String("&amp;");
        } else if (ch == QLatin1Char('"')) {
            result += QLatin1String("&quot;");
        } else if (ch == QLatin1Char('<')) {
            result += QLatin1String("&lt;");
        } else if (ch == QLatin1Char('>')) {
            result += QLatin1String("&gt;");
        } else {
            const int start = helper.mPos;
            if (!(flags & IgnoreUrls)) {
                bool badUrl = false;
                str = helper.getUrl(&badUrl);
                if (badUrl) {
                    QString resultBadUrl;
                    const int helperTextSize(helper.mText.count());
                    for (int i = 0; i < helperTextSize; ++i) {
                        const QChar chBadUrl = helper.mText.at(i);
                        if (chBadUrl == QLatin1Char('&')) {
                            resultBadUrl += QLatin1String("&amp;");
                        } else if (chBadUrl == QLatin1Char('"')) {
                            resultBadUrl += QLatin1String("&quot;");
                        } else if (chBadUrl == QLatin1Char('<')) {
                            resultBadUrl += QLatin1String("&lt;");
                        } else if (chBadUrl == QLatin1Char('>')) {
                            resultBadUrl += QLatin1String("&gt;");
                        } else {
                            resultBadUrl += chBadUrl;
                        }
                    }
                    return resultBadUrl;
                }
                if (!str.isEmpty()) {
                    QString hyperlink;
                    if (str.startsWith(QLatin1String("www."))) {
                        hyperlink = QLatin1String("http://") + str;
                    } else if (str.startsWith(QLatin1String("ftp."))) {
                        hyperlink = QLatin1String("ftp://") + str;
                    } else {
                        hyperlink = str;
                    }
                    if (hyperlink.endsWith(QLatin1Char('"'))) {
                        hyperlink.chop(1);
                    }
                    if (str.endsWith(QLatin1Char('"'))) {
                        str.chop(1);
                    }
                    result += QLatin1String("<a href=\"") + hyperlink + QLatin1String("\">") + str.toHtmlEscaped() + QLatin1String("</a>");
                    x += helper.mPos - start;
                    continue;
                }
                str = helper.getEmailAddress();
                if (!str.isEmpty()) {
                    // len is the length of the local part
                    int len = str.indexOf(QLatin1Char('@'));
                    QString localPart = str.left(len);

                    // remove the local part from the result (as '&'s have been expanded to
                    // &amp; we have to take care of the 4 additional characters per '&')
                    result.truncate(result.length() - len - (localPart.count(QLatin1Char('&')) * 4));
                    x -= len;

                    result += QLatin1String("<a href=\"mailto:") + str + QLatin1String("\">") + str + QLatin1String("</a>");
                    x += str.length() - 1;
                    continue;
                }
                if (flags & ConvertPhoneNumbers) {
                    str = helper.getPhoneNumber();
                    if (!str.isEmpty()) {
                        result += QLatin1String("<a href=\"tel:") + normalizePhoneNumber(str) + QLatin1String("\">") + str + QLatin1String("</a>");
                        x += str.length() - 1;
                        continue;
                    }
                }
            }
            if (flags & HighlightText) {
                str = helper.highlightedText();
                if (!str.isEmpty()) {
                    result += str;
                    x += helper.mPos - start;
                    continue;
                }
            }
            result += ch;
        }
    }

    return result;
}
