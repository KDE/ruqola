#! /usr/bin/env bash
$XGETTEXT `find . -name "*.cpp" -o -name "*.qml" -o -name "*.ui" -name "*.h"` -L Java -o $podir/ruqola.pot
