#/bin/bash
echo Target is $1
echo Folder is $2

if [ -n "$1" ]; then
    if [ ! -n "$2" ]; then
        find . -iname \*.cpp -exec grep --color -wn -H "$1" \{\} \;
        find . -iname \*.[ch] -exec grep --color -wn -H "$1" \{\} \;
    else
        find $2 -iname \*.cpp -exec grep --color -wn -H "$1" \{\} \;
        find $2 -iname \*.[ch] -exec grep --color -wn -H "$1" \{\} \;
    fi
fi
