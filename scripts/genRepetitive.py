#!/usr/bin/env python3

for x in range(0, 48):
	if(x in set([8, 10, 11, 12, 13, 14, 17])):
		print('ISR_ERRCODE', x)
	else:
		print('ISR_NOERRCODE', x)

#for x in range(32, 32+16):
#	print('IRQ, 

print('\n\n')

for x in range(0, 48):
	print('idtEntries[{0}].setGate((std::uint32_t)isr{0}, 0x08, true, 3);'.format(x))

print('\n\n')

for x in range(0, 48):
	print('extern "C" void isr{0}();'.format(x))
