default_fqbn := "adafruit:avr:itsybitsy32u4_3V"
default_port := `ls -1 /dev/cu.usb*|head -1`
default_sketch := "thermdet"

_list:
  @just -l

@ports:
  ls -1 /dev/cu.usb*

@build sketch=default_sketch fqbn=default_fqbn +flags="":
  echo Building {{sketch}}
  @arduino-cli compile --libraries {{justfile_directory()}}/libraries -b {{fqbn}} {{flags}} sketches/{{sketch}}/{{sketch}}.ino

@upload sketch=default_sketch fqbn=default_fqbn port=default_port flags="":
  just build {{sketch}} {{fqbn}} --upload -p {{port}} {{flags}}

@monitor port=default_port:
  arduino-cli monitor -p {{port}} --timestamp
