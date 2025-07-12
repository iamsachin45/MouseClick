# MouseClick - A simple linux kernel that prints the mouse click event. 

To run: 
1. first do `make`
2. and then insert the module by `sudo insmod MouseClick.ko`
3. Then simply do `dmesg`
4. If you encounter any issue like permission denied, allow temp permission by `sudo sysctl kernel.dmesg_restrict=0`



