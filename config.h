#define PLUG_NAME "HISSTools Granular"
#define PLUG_MFR "Alex Harker"
#define PLUG_VERSION_HEX 0x00010000
#define PLUG_VERSION_STR "1.0.0"
#define PLUG_UNIQUE_ID 'HTGn'
#define PLUG_MFR_ID 'AHAR'
#define PLUG_URL_STR "www.alexanderjharker.co.uk"
#define PLUG_EMAIL_STR "A.Harker@hud.ac.uk"
#define PLUG_COPYRIGHT_STR  "Copyright 2018 Alex Harker"
#define PLUG_CLASS_NAME HISSToolsGranular

#define BUNDLE_NAME "HISSToolsGranular"
#define BUNDLE_MFR "AlexHarker"
#define BUNDLE_DOMAIN "co.uk"

#define AUV2_ENTRY HISSToolsGranular_Entry
#define AUV2_ENTRY_STR "HISSToolsGranular_Entry"
#define AUV2_FACTORY HISSToolsGranular_Factory
#define AUV2_VIEW_CLASS HISSToolsGranular_View
#define AUV2_VIEW_CLASS_STR "HISSToolsGranular_View"

#define PLUG_CHANNEL_IO "1-2 2-2"
#define PLUG_LATENCY 0
#define PLUG_TYPE 0
#define PLUG_DOES_MIDI_IN 0
#define PLUG_DOES_MIDI_OUT 0
#define PLUG_DOES_MPE 0
#define PLUG_DOES_STATE_CHUNKS 1
#define PLUG_FPS 60
#define PLUG_HAS_UI 1
#define PLUG_SHARED_RESOURCES 0

#define APP_SIGNAL_VECTOR_SIZE 64

#define PLUG_WIDTH 945
#define PLUG_HEIGHT 645

#if defined(AAX_API) && !defined(_PIDS_)
#define _PIDS_
const int PLUG_TYPE_IDS[2] = {'EFN1', 'EFN2'};
const int PLUG_TYPE_IDS_AS[2] = {'EFA1', 'EFA2'}; // AudioSuite
#endif
#define AAX_PLUG_MFR_STR "AlexHarker\nAlexHarker\nAcme"
#define AAX_PLUG_NAME_STR "HISSToolsGranular\nIPEF"
#define AAX_PLUG_CATEGORY_STR "Effect"
#define AAX_DOES_AUDIOSUITE 1

#define VST3_SUBCATEGORY "Effect"

#define APP_ENABLE_SYSEX 0
#define APP_ENABLE_MIDICLOCK 0
#define APP_ENABLE_ACTIVE_SENSING 0
#define APP_NUM_CHANNELS 2
#define APP_N_VECTOR_WAIT 50
#define APP_MULT 1.0

