#!/bin/bash
ID_REGEX="([0-9]|[A-Z][0-9][0-9A-Z]*)[0-9A-Z]*"

cat $1 | sed -E "s/.*([rR]equisites?: [^.]*\.).*/\1/" | sed -E "s/^[^rR].*//" \
    | sed -E "s/ ?\([^)]*\)//g" | sed -E "s/^[rR]equisites?: [^c].*//" \
    | sed -E "s/,? and/,/" | sed -E "s/(${ID_REGEX})( or ((\w+ )+)?(${ID_REGEX}))+/\1/g" \
    | sed -E "s/,? with grades? of [A-F]-?( or better)?//" \
    | sed -E "s/ or .*//" \
    | sed -E "s/[rR]equisites?: courses?//" \
    | sed -E "s/one \w+ course from ((\w+ )+${ID_REGEX})(, ${ID_REGEX})+/\1/" \
    | sed -E "s/one \w+ course from (([A-Za-z]+ )+${ID_REGEX})(, ([A-Za-z]+ )+${ID_REGEX})+/\1/" \
    | sed -E "s/\.$//"
