#! /bin/sh

BASEDIR=$(dirname $0)

EXPORT_PATH="$BASEDIR/../build-mac/HISSTools_Granular"
ZIP_PATH="$EXPORT_PATH.zip"

find "$EXPORT_PATH" -name '.DS_Store' -type f -delete || exit 1
find "$EXPORT_PATH" -name '._*' -type f -delete || exit 1

/usr/bin/ditto -c -k --norsrc --keepParent "$EXPORT_PATH" "$ZIP_PATH" || exit 1
