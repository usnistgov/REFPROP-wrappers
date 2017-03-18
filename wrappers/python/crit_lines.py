from __future__ import print_function
import numpy as np, matplotlib.pyplot as plt, os, glob
from matplotlib.backends.backend_pdf import PdfPages
from ctypes_wrapper import REFPROPFunctionLibrary
import shutil

r = REFPROPFunctionLibrary(r'C:\Users\ihb\Documents\Code\REFPROP-cmake\build\10\Release\REFPRP64.dll')

def get_binaries(fluids_dir):
    """ Get all the binaries that can be found in the HMX.BNC file """
    def get_CAS_map():
        o = {}
        for fld in glob.glob(os.path.join(fluids_dir, '*.FLD')):
            with open(fld, 'r') as fp:
                lines = fp.readlines()
                CAS = lines[1].split('!')[0].strip()
                o[CAS] = os.path.split(fld)[1].split('.')[0]
        return o
    CAS_map = get_CAS_map()

    with open(os.path.join(fluids_dir,'HMX.BNC'), 'r') as fp:
        contents = fp.read()
    chunks = contents.split('\n!\n')
    mixes = []
    bad_keys = []
    for chunk in chunks:
        if '#BNC' in chunk: continue
        if not chunk.startswith('?'): continue
        if '#MXM' in chunk: break
        lines = chunk.split('\n')
        CAS1, CAS2 = lines[2][0:26].strip().split('/')
        n1,n2 = None,None
        try:
            n1, n2 = CAS_map[CAS1], CAS_map[CAS2]
        except KeyError as K:
            bad_keys.append(str(K))
            continue

        if 'TC' in chunk and 'VC' in chunk:
            mixes.append((n1, n2))
        else:
            pass
            #print('mystery', n1, n2, chunk)
    print('bad keys:', sorted(set(bad_keys)))
    return mixes

def all_binaries(mixes):
    pdfTypeI = PdfPages('TypeIcrits.pdf')
    pdfNotTypeI = PdfPages('NotTypeIcrits.pdf')
    
    for mix in mixes:
        ierr, herr = r.SETUPdll(2, '|'.join(mix), 'HMX.BNC', 'DEF')
        if ierr > 0: ValueError()

        z = np.zeros((20,))
        Tc_guess,Pc_guess,rhoc_mol_L_guess = -1.0,-1.0,-1.0

        Tthermo,Vthermo,Tfit,Vfit,Z0 = [],[],[],[],[]
        for z0 in np.linspace(0, 1, 1000):

            z[0] = z0; z[1] = 1-z0
            # Evaluation of the "thermodynamically consistent" critical point
            # from the methods in Michelsen
            Tc,Pc,rhoc_mol_L,ierr,herr = r.CRTPNTdll(z, Tc_guess, Pc_guess, rhoc_mol_L_guess)
            if ierr > 0:
                # Update guesses
                Tc_guess = Tc; Pc_guess = Pc; rhoc_mol_L_guess = rhoc_mol_L
                Tthermo.append(Tc); Vthermo.append(1/rhoc_mol_L)
                # Evaluation of the fitted value that is in REFPROP
                Tc2,Pc2,rhoc_mol_L2,ierr2,herr2 = r.CRITPdll(z, Tc, Pc, rhoc_mol_L)
                Tfit.append(Tc2); Vfit.append(1/rhoc_mol_L2)
                Z0.append(z0)

        Tthermo, Tfit = [np.array(_) for _ in [Tthermo, Tfit]]
        errT = np.array((Tthermo - Tfit)/Tfit*100)
        RMST = np.sqrt(np.sum(np.power(errT,2)))
        
        fig,(ax1,ax2) = plt.subplots(1,2,figsize=(10,5))
        plt.suptitle(' & '.join(mix))
        
        ax1.plot(Z0, Tthermo, 'o')
        ax1.plot(Z0, Tfit, '-')
        ax2.plot(Z0, Vthermo, 'o')
        ax2.plot(Z0, Vfit, '-')

        for ax in ax1,ax2:
            ax.set_xlabel(r'$z_{\rm '+str(mix[0])+'}$ (mol/mol)')
        ax1.set_ylabel('T (K)')
        ax2.set_ylabel('v (L/mol)')
        #fig.tight_layout(pad=0.2)
        if RMST < 3:
            pdfTypeI.savefig(fig)
        else:
            pdfNotTypeI.savefig(fig)
        plt.close(fig)
        del fig
        print(mix, RMST)

    pdfTypeI.close()
    pdfNotTypeI.close()

def collect(fluids_path, HMX_path):
    shutil.copytree(fluids_path, 'fluids')
    shutil.copy2(os.path.join(HMX_path, 'HMX.BNC'), os.path.join('fluids', 'HMX.BNC'))

if __name__=='__main__':
    collect(r'R:\FLUIDS', r'R:\V10\HMX')
    mixes = get_binaries(r'fluids')
    print(len(mixes),'Type I mixtures w/ BIP found')
    all_binaries(mixes)
    shutil.rmtree('fluids')