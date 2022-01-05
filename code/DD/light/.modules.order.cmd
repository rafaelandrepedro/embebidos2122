cmd_/home/bruno/code/DeviceDriver/light/modules.order := {   echo /home/bruno/code/DeviceDriver/light/light.ko; :; } | awk '!x[$$0]++' - > /home/bruno/code/DeviceDriver/light/modules.order
