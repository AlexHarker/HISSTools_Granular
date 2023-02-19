#! /bin/sh

# Path Names

EXPORT_PATH="$1"
PLUG_NAME="$2"

TIME=`date +"%d-%m-%y-%T"`
TIMESTAMP="${TIME//:/}"

FILE="${PLUG_NAME//./-}"
NOTIFY_NAME="${FILE// /}-$TIMESTAMP"

PLUG_PATH="$3/$PLUG_NAME"
ZIP1_PATH="$EXPORT_PATH/$NOTIFY_NAME.zip"
ZIP2_PATH="$EXPORT_PATH/OSX/$PLUG_NAME.zip"

# Create a ZIP archive suitable for notarytool

codesign --force --timestamp --sign "Developer ID Application" "$PLUG_PATH"
/usr/bin/ditto -c -k --keepParent "$PLUG_PATH" "$ZIP1_PATH" || exit 1

# Notarise

if ! xcrun notarytool submit "$ZIP1_PATH" --keychain-profile "AJH-NOTARISE" --wait
then
  rm "$ZIP1_PATH"
  exit 1
fi

rm "$ZIP1_PATH"

# Staple

xcrun stapler staple "$PLUG_PATH" || exit 1

/usr/bin/ditto -c -k --keepParent "$PLUG_PATH" "$ZIP2_PATH" || exit 1
