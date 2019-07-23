# drv10987-driver

This highly unfinished driver for the DRV10987 3-phase sensorless BLDC motor
driver is released here with the hope that it becomes useful to someone someday.
I quit developing it after prototype tests revealed that the IC wasn't suitable
for my application, so it's left in a weird state with most of the interface
missing and the rest tightly coupled to the communication protocol drivers of my
specific microcontroller. A lot of the planned driver functionality is missing
as well. Register maps are mostly there (though some aren't where they are
supposed to be) and could be adapted with minimal work. Use at your own risk.
