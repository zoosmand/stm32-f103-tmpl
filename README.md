# STM32F103 Template Project

## STM32F103C8T6

### Tips and Tricks

* Add udev rules.
~~~ bash
cat << EOF | sudo tee /etc/udev/rules.d/90-st-link.rules
SUBSYSTEM=="usb", ATTRS{idVendor}=="0483", ATTRS{idProduct}=="3748", MODE="0666", OWNER="1000" 
EOF

sudo udevadm control --reload-rules
~~~

### Useful Links


---

&copy; 2017-2025, Askug Ltd.
