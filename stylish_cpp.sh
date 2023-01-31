#!/bin/sh

SOURCE_FOLDERS="./demo1src ./test_lightest ./src"
SOURCE_MASKS='(c|cpp|cxx|c++|h|hpp|hxx|h++)'

find ${SOURCE_FOLDERS} -type f -regextype posix-egrep -regex ".+\.${SOURCE_MASKS}$" | \
while read source; do \
    rm -f ${source}.bak
    cp ${source} ${source}.bak || continue
	clang-format -i --verbose ${source}
done
