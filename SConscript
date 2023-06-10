import os
from building import *

cwd = GetCurrentDir()
objs = []
list = os.listdir(cwd)

# add general drivers
src = Split('''
src/si446x.c
''')



if GetDepend('PKG_USING_SI446X'):
    path =  [cwd]
    path += [cwd + '/inc']
    group = DefineGroup('Si446x', src, depend = [''], CPPPATH = path)

    
Return('objs')