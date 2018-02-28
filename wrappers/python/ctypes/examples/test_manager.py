import timeit

import sys
sys.path.insert(0,'..')
import ctREFPROP.ctREFPROP as ct

path = r"D:\Code\REFPROP-cmake\build\10dyn\Release"
RP1 = ct.REFPROPFunctionLibrary(path, "dll")

manager = ct.REFPROPLibraryManager(r'D:\Code\REFPROP-manager\msvc\Release\REFMAN64.dll')
N = 100
tic = timeit.default_timer()
RPs = [manager.get_instance(path,"REFPRP64.dll") for _counter_ in range(N)]
toc = timeit.default_timer()
print((toc-tic)/N, 's: average time to load a copy of REFPROP')

def do_stuff(RP):
    RP.SETPATHdll(path)

    baseSI = RP.GETENUMdll(0, "MOLAR BASE SI  ").iEnum
    iMass = 0; iFlag = 0
    zz = [1.0]+[0.0]*19
    r = RP.REFPROPdll(r"AMMONIA.FLD", "TD","ETA",baseSI,iMass,iFlag,398.788,7044.7,zz)
    print(r)

    RP.SETUPdll(2,"Methane&Ethane","HMX.BNC","DEF")
    r = RP.SATTdll(300, [0.5,0.5], 0)
    print(r)
    print(RP.managed_handle)

do_stuff(RP1)
do_stuff(RPs[N//2])