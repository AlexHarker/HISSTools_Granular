#!/usr/bin/python

# this script will copy resources to the ~/Music/PLUG_NAME folder or the bundle depending on PLUG_SHARED_RESOURCES

import plistlib, os, datetime, fileinput, glob, sys, string, shutil

scriptpath = os.path.dirname(os.path.realpath(__file__))
projectpath = os.path.abspath(os.path.join(scriptpath, os.pardir))

IPLUG2_ROOT = "../../.."

sys.path.insert(0, os.path.join(os.getcwd(), IPLUG2_ROOT + '/Scripts'))

from parse_config import parse_config

def main():
  config = parse_config(projectpath)

  print "Copying resources ..."

  if config['PLUG_SHARED_RESOURCES']:
    dst = os.path.expanduser("~") + "/Music/" + config['BUNDLE_NAME'] + "/Resources"
  else:
    dst = os.environ["TARGET_BUILD_DIR"] + os.environ["UNLOCALIZED_RESOURCES_FOLDER_PATH"]

  if os.path.exists(dst) == False:
    os.makedirs(dst + "/", 0755 )

  if os.path.exists(projectpath + "/resources/img/"):
    imgs = os.listdir(projectpath + "/resources/img/")
    for img in imgs:
      print "copying " + img + " to " + dst
      shutil.copy(projectpath + "/resources/img/" + img, dst)

  if os.path.exists(projectpath + "/resources/fonts/"):
    fonts = os.listdir(projectpath + "/resources/fonts/")
    for font in fonts:
      print "copying " + font + " to " + dst
      shutil.copy(projectpath + "/resources/fonts/" + font, dst)

if __name__ == '__main__':
  main()
