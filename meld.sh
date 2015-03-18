#!/bin/sh
#
# Download Meld to homedir and run git mergetool.
#

MELD_HOME_DIR="$HOME/meld"
MELD_BIN="/usr/bin/meld"

if [ ! -f "$MELD_BIN" ]; then
    if [ ! -d "$MELD_HOME_DIR" ]; then
	    echo "You don't have meld installed. Let me get that for you ..."
	    git clone https://git.gnome.org/browse/meld $MELD_HOME_DIR
    fi
    MELD_BIN="$MELD_HOME_DIR/bin/meld"
fi

# We should have a Meld binary by now.
if [ -f "$MELD_BIN" ]; then
    GIT_CMD="git mergetool --tool=\"$MELD_BIN\""
    $GIT_CMD
    if [ $? -eq 0 ]; then
	echo "If you need to run Meld again you can do so with '$GIT_CMD'"
    fi
else
    echo "Can't find $MELD_BIN even after looking for it and trying to install it. You're on your own."
fi
