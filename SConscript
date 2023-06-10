import os
from building import *

cwd = GetCurrentDir()
objs = []
list = os.listdir(cwd)

# add general drivers
src = Split('''
src/si446x.c
src/radio_comm.c
src/radio.c
src/si446x_api_lib.c
src/spi.c
''')



if GetDepend('PKG_USING_SI446X'):
    path =  [cwd]
    path += [cwd + '/inc']
    group = DefineGroup('Drivers', src, depend = [''], CPPPATH = path)

    
Return('objs')