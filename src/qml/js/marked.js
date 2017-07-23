/*
 * <one line to give the program's name and a brief idea of what it does.>
 * Copyright 2016  Riccardo Iaconelli <riccardo@kde.org>
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License as
 * published by the Free Software Foundation; either version 2 of
 * the License or (at your option) version 3 or any later version
 * accepted by the membership of KDE e.V. (or its successor approved
 * by the membership of KDE e.V.), which shall act as a proxy
 * defined in Section 14 of version 3 of the license.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 *
 */


.pragma library;

function md2html (arg) {
    var tmp = ({});

    // Make sure text begins and ends with a couple of newlines:
    var ret = ('\n\n\n' + (arg || "") + '\n\n\n');

    // NEWLINE NORMALIZATION
    ret = ret.replace (/\r\n/gmi, '\n'); // replace WIN CRLF with LF
    ret = ret.replace (/\r/gmi,   '\n'); // replace MAC CR with LF

    // isolate PRE tags to not touch their content
    var listPre = ret.match (/```\n([^```]+)\n```/gmi);
    if (listPre) {
        for (var idxPre = 0; idxPre < listPre.length; idxPre++) {
            var preKey = '![PRE ' + idxPre + ']';
            var preMd = listPre [idxPre];
            ret = ret.replace (preMd, preKey);
            var preHtml = preMd;
            preHtml = preHtml.replace (/&/gi, "&amp;");
            preHtml = preHtml.replace (/</gi, "&lt;");
            preHtml = preHtml.replace (/>/gi, "&gt;");
            preHtml = preHtml.replace (/\t/gmi, '    '); // tab = 4 spaces
            preHtml = preHtml.replace (/```\n([^```]+)\n```/gmi, '<pre>$1</pre>'); // ```\ntext\n``` = <pre>text</pre>
            tmp [preKey] = preHtml;
        }
    }

    // isolate CODE tags to not touch their content
    var listCode = ret.match (/`([^`\n]+)`/gi);
    if (listCode) {
        for (var idxCode = 0; idxCode < listCode.length; idxCode++) {
            var codeKey = '![CODE ' + idxCode + ']';
            var codeMd = listCode [idxCode];
            ret = ret.replace (codeMd, codeKey);
            var codeHtml = codeMd;
            codeHtml = codeHtml.replace (/&/gi, "&amp;");
            codeHtml = codeHtml.replace (/</gi, "&lt;");
            codeHtml = codeHtml.replace (/>/gi, "&gt;");
            codeHtml = codeHtml.replace (/`([^`\n]+)`/gi, '<code>$1</code>'); // `text` = <code>text</code>
            tmp [codeKey] = codeHtml;
        }
    }

    // isolate AUTO-LINK tags to not ruin urls later
    var listLink = ret.match (/<([^>]*)>/gi);
    if (listLink) {
        for (var idxLink = 0; idxLink < listLink.length; idxLink++) {
            var linkKey = '![LINK ' + idxLink + ']';
            var linkMd = listLink [idxLink];
            ret = ret.replace (linkMd, linkKey);
            var linkHtml = linkMd;
            linkHtml = linkHtml.replace (/<([^>]*)>/gi, '<a href="$1" target="_new">$1</a>'); // <url> = <a href="url">url</a>
            tmp [linkKey] = linkHtml;
        }
    }
    //ret = ret.replace (/!\[LINK ([^\n]+)\]/gmi, '<a href="$1" target="_new">$1</a>'); // ![LINK url] = <a href="url">url</a>

    // IMAGE
    var listImg = ret.match (/!\[IMG ([^\n]+)\]/gi);
    if (listImg) {
        for (var idxImg = 0; idxImg < listImg.length; idxImg++) {
            var imgKey = '![IMG ' + idxImg + ']';
            var imgMd = listImg [idxImg];
            ret = ret.replace (imgMd, imgKey);
            var imgHtml = imgMd;
            imgHtml = imgHtml.replace (/!\[IMG ([^\n]+)\]/gi, '<img src="$1" />'); // ![IMG url] = <img src="url" />
            tmp [imgKey] = imgHtml;
        }
    }

    // HTML ENTITY ESCAPING
    ret = ret.replace (/&/gi, '&amp;');
    ret = ret.replace (/</gi, '&lt;');
    ret = ret.replace (/>/gi, '&gt;');

    // TODO MARKER
    ret = ret.replace (/^!\[TODO\](.*)$/gmi, '<span class="todo">TODO $1</span>'); // ![TODO] XXXX = <span class="todo">TODO XXXXX</span>

    // HEADERS
    ret = ret.replace (/^######\s*(.+)\s*\n/gmi, '<h6 class="heading">$1</h6>\n');
    ret = ret.replace (/^#####\s*(.+)\s*\n/gmi, '<h5 class="heading">$1</h5>\n');
    ret = ret.replace (/^####\s*(.+)\s*\n/gmi, '<h4 class="heading">$1</h4>\n');
    ret = ret.replace (/^###\s*(.+)\s*\n/gmi, '<h3 class="heading">$1</h3>\n');
    ret = ret.replace (/^##\s*(.+)\s*\n/gmi, '<h2 class="heading">$1</h2>\n');
    ret = ret.replace (/^#\s*(.+)\s*\n/gmi, '<h1 class="heading">$1</h1>\n');
    ret = ret.replace (/^\s*(.+)\s*\n====*\s*\n/gmi, '<div class="title">$1</div>\n'); // text\n==== = TEXT

    // EMPHASIS
    ret = ret.replace (/([^\w]+)\*\*([^\*\*]+)\*\*([^\w]+)/gi, '$1<strong>$2</strong>$3'); // **text** = <strong>text</strong>
    ret = ret.replace (/([^\w]+)__([^__]+)__([^\w]+)/gi, '$1<strong>$2</strong>$3'); // __text__ = <strong>text</strong>
    ret = ret.replace (/([^\w]+)\*([^\*]+)\*([^\w]+)/gi, '$1<em>$2</em>$3'); // *text* = <em>text</em>
    ret = ret.replace (/([^\w]+)_([^_]+)_([^\w]+)/gi, '$1<em>$2</em>$3'); // _text_ = <em>text</em>

    // LIST
    ret = ret.replace (/^\s*\*\s*([^\n]+)\n/gmi, '<ul><li>$1</li></ul>');
    ret = ret.replace (/^\s*\+\s*([^\n]+)\n/gmi, '<ul><li>$1</li></ul>');
    ret = ret.replace (/^\s*\-\s*([^\n]+)\n/gmi, '<ul><li>$1</li></ul>');
    ret = ret.replace (/^\s*\d\.\s*([^\n]+)\n/gmi, '<ol><li>$1</li></ol>')

    // remove extra consecutive list separators
    ret = ret.replace (/<\/ul>\n*<ul>/gmi, '');
    ret = ret.replace (/<\/ol>\n*<ol>/gmi, '');

    // PARAGRAPH
    ret = ret.trim ();
    ret = ret.replace (/\n/gmi, '<br />'); // \n = <br />
    ret = ret.replace (/\t/gmi, '&nbsp;&nbsp;&nbsp;&nbsp;'); // tabs = 4 unbreakable spaces
    //ret = ret.replace (/\s+/gmi, " "); // remove multiple spaces between words

    // TOC PLACEHOLDER
    ret = ret.replace (/!\[TOC\]/gmi, '<div id="TOC"></div>');

    // put back verbatim contents
    for (var key in tmp) {
        ret = ret.replace (key, tmp [key]);
    }

    // RETURN HTML FRAGMENT
    return ret;
}
