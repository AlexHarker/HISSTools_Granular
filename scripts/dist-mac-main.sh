#! /bin/sh

BASEDIR=$(dirname $0)

EXPORT_PATH="$BASEDIR/../build-mac/"
AUDIO_PATH="$HOME/Library/Audio/Plug-Ins"

if [ ! -d "$EXPORT_PATH" ]
then
  mkdir "$EXPORT_PATH"
fi

if [ ! -d "$EXPORT_PATH/OSX" ]
then
  mkdir "$EXPORT_PATH/OSX"
fi

"$BASEDIR/dist-mac-notarize.sh" "$EXPORT_PATH" HISSToolsGranular.component "$AUDIO_PATH/Components" || exit 1
"$BASEDIR/dist-mac-notarize.sh" "$EXPORT_PATH" HISSToolsGranular.vst "$AUDIO_PATH/VST" || exit 1
"$BASEDIR/dist-mac-notarize.sh" "$EXPORT_PATH" HISSToolsGranular.vst3 "$AUDIO_PATH/VST3" || exit 1
