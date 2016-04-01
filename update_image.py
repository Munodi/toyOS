import os
import subprocess
import sys
if os.name == "posix":
    print("posix")
    subprocess.call("sudo /sbin/losetup /dev/loop0 floppy.img", shell=True)
    subprocess.call("sudo mount /dev/loop0 /mnt", shell=True)
    subprocess.call("sudo cp kernel /mnt/kernel", shell=True)
    subprocess.call("sudo umount /dev/loop0", shell=True)
    subprocess.call("sudo /sbin/losetup -d /dev/loop0", shell=True)
elif os.name == "nt":
    print("nt: not implemented yet")
else:
    print("unknown os")
