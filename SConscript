import os
from building import *

cwd = GetCurrentDir()
objs = []
list = os.listdir(cwd)

# add general drivers
src = Split('''
src/drv_si446x.c
src/radio_comm.c
src/radio.c
src/si446x_api_lib.c
src/spi.c
''')



if GetDepend('PKG_USING_SI446X'):
    path =  [cwd]
    path += [cwd + '/inc']
    group = DefineGroup('Si446x', src, depend = [''], CPPPATH = path)

    
Return('objs')