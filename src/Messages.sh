#! /usr/bin/env bash
$EXTRACTRC `find . -name "*.ui" -o -name "*.rc"` >> rc.cpp
$XGETTEXT `find . -name "*.cpp"` -o $podir/ruqola.pot
