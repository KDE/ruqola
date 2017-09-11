function stringToColour(str) {
    var hash = 0;
    for (var i = 0; i < str.length; i++) {
        hash = str.charCodeAt(i) + ((hash << 5) - hash);
    }
    var colour = '#';
    for (var j = 0; j < 3; j++) {
        var value = (hash >> (j * 8)) & 0xFF;
        colour += ('00' + value.toString(16)).substr(-2);
    }
    return colour;
}

function displayDateTime(timestamp)
{
    return Qt.formatDate(new Date(timestamp), i18n("yyyy-MM-dd")) + "\n" + Qt.formatTime(new Date(timestamp), i18n("hh:mm"));
}

/*
* Author: Johnny Broadway <johnny@johnnybroadway.com>
* Website: https://gist.github.com/jbroadway/2836900
* License: MIT
*/
function slimdown() {

    // Rules
    this.rules =  [
             {regex: /\n(#+)(.*)/g, replacement: header},                                         // headers
             {regex: /!\[([^\[]+)\]\(([^\)]+)\)/g, replacement: '<img src=\'$2\' alt=\'$1\'>'}, // image
             {regex: /\[([^\[]+)\]\(([^\)]+)\)/g, replacement: '<a href=\'$2\'>$1</a>'},        // hyperlink
             //{regex: /(_)(.*?)\1/g, replacement: '<i>$2</i>'},                             // emphasis
             {regex: /\~\~(.*?)\~\~/g, replacement: '<del>$1</del>'},                           // del
             {regex: /\:\"(.*?)\"\:/g, replacement: '<q>$1</q>'},                               // quote
             {regex: /`(.*?)`/g, replacement: '<code>$1</code>'},                               // inline code
             //{regex: /\n\*(.*)/g, replacement: ulList},                                         // ul lists
             {regex: /\n[0-9]+\.(.*)/g, replacement: olList},                                   // ol lists
             {regex: /\n(&gt;|\>)(.*)/g, replacement: blockquote},                              // blockquotes
             {regex: /\n-{5,}/g, replacement: '\n<hr />'},                                      // horizontal rule
             {regex: /\n([^\n]+)\n/g, replacement: para},                                       // add paragraphs
             {regex: /<\/ul>\s?<ul>/g, replacement: ''},                                        // fix extra ul
             {regex: /<\/ol>\s?<ol>/g, replacement: ''},                                        // fix extra ol
             {regex: /<\/blockquote><blockquote>/g, replacement: '\n'}                          // fix extra blockquote
         ];

    // Add a rule.
    this.addRule = function (regex, replacement) {
        regex.global = true;
        regex.multiline = false;
        this.rules.push({regex: regex, replacement: replacement});
    };

    // Render some Markdown into HTML.
    this.render = function (text) {
        text = '\n' + text + '\n';
        this.rules.forEach(function (rule) {
            text = text.replace(rule.regex, rule.replacement);
        });
        return text.trim();
    };

    function para (text, line) {
        debugger;
        var trimmed = line.trim();
        if (/^<\/?(ul|ol|li|h|p|bl)/i.test(trimmed)) {
            return '\n' + line + '\n';
        }
        return '\n<p>' + trimmed + '</p>\n';
    }

    function ulList (text, item) {
        return '\n<ul>\n\t<li>' + item.trim() + '</li>\n</ul>';
    }

    function olList (text, item) {
        return '\n<ol>\n\t<li>' + item.trim() + '</li>\n</ol>';
    }

    function blockquote (text, tmp, item) {
        return '\n<blockquote>' + item.trim() + '</blockquote>';
    }

    function header (text, chars, content) {
        var level = chars.length;
        return '<h' + level + '>' + content.trim() + '</h' + level + '>';
    }
}


function markdownme(s) {
    //         var md = MarkDown.markdownit();
    //         var result = md.render('# markdown-it rulezz!');
    var sd = new slimdown();
    var result= sd.render(s);

    //         var regex = new RegExp(/\[([^\[]+)\]\(([^\)]+)\)/g);
    //         result = s.replace(regex, '<a href=\'$2\'>$1</a>');

    var regex2 = new RegExp(/#(\w+(?:\.\w+)?)/g);
    result = result.replace(regex2, '<a href=\'ruqola:/room/$1\'>#$1</a>');

    var regex3 = new RegExp(/@(\w+(?:\.\w+)?)/g);
    result = result.replace(regex3, '<a href=\'ruqola:/user/$1\'>@$1</a>');

    console.log(result)

    //         var regex = new RegExp(/\[([^\[]+)\]\(([^\)]+)\)/g);
    //         var result = s.replace(regex, '<a href=\'$2\'>$1</a>');


    return result;
}
