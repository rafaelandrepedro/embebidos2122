cmd_/home/bruno/code/DeviceDriver/waterpump/Module.symvers := sed 's/ko$$/o/' /home/bruno/code/DeviceDriver/waterpump/modules.order | scripts/mod/modpost -m -a   -o /home/bruno/code/DeviceDriver/waterpump/Module.symvers -e -i Module.symvers   -T -
