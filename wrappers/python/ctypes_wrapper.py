from __future__ import print_function
import ctypes as ct
import sys

def trim(s):
    return s.replace(b'\x00',b'').strip().decode('utf-8')

class REFPROPFunctionLibrary():
    def __init__(self, name):
        if sys.platform.startswith('win'):
            self.dll = ct.WinDLL(name)
        else:
            self.dll = ct.CDLL(name)

        self._SETUPdll = getattr(self.dll, 'SETUPdll')
        self._SETPATHdll = getattr(self.dll, 'SETPATHdll')
        self._INFOdll = getattr(self.dll, 'INFOdll')
        self._SETREFdll = getattr(self.dll, 'SETREFdll')
        self._LIMITSdll = getattr(self.dll, 'LIMITSdll')
        self._MELTTdll = getattr(self.dll, 'MELTTdll')
        self._THERMdll = getattr(self.dll, 'THERMdll')
        self._RESIDUALdll = getattr(self.dll, 'RESIDUALdll')
        self._TDFLSHdll = getattr(self.dll, 'TDFLSHdll')
        self._TPFLSHdll = getattr(self.dll, 'TPFLSHdll')
        self._PEFLSHdll = getattr(self.dll, 'PEFLSHdll')
        self._CRTPNTdll = getattr(self.dll, 'CRTPNTdll')
        self._CRITPdll = getattr(self.dll, 'CRITPdll')
        self._SATTdll = getattr(self.dll, 'SATTdll')
        self._SATTPdll = getattr(self.dll, 'SATTPdll')
        self._TQFLSHdll = getattr(self.dll, 'TQFLSHdll')
        self._WMOLdll = getattr(self.dll, 'WMOLdll')
        self._GETMODdll = getattr(self.dll, 'GETMODdll')
        self._REDXdll = getattr(self.dll, 'REDXdll')
        self._FLAGSdll = getattr(self.dll, 'FLAGSdll')
        self._RMIX2dll = getattr(self.dll, 'RMIX2dll')
        self._TRNPRPdll = getattr(self.dll, 'TRNPRPdll')
        self._TPRHOdll = getattr(self.dll, 'TPRHOdll')
        self._SURFTdll = getattr(self.dll, 'SURFTdll') 
        self._NAMEdll = getattr(self.dll, 'NAMEdll')         

    def FLAGSdll(self, iFlag,jFlag):
        """ subroutine FLAGS (iFlag,jFlag,kFlag,ierr,herr) """
        iflag = ct.c_long(iFlag)
        jflag = ct.c_long(jFlag)
        kflag = ct.c_long(0)
        ierr = ct.c_long(0)
        herr = ct.create_string_buffer(255)
        self._FLAGSdll(ct.byref(iflag), ct.byref(jflag), ct.byref(kflag), ct.byref(ierr), herr)
        return kflag.value, ierr.value, trim(herr.raw)

    def REDXdll(self, z):
        z = (len(z)*ct.c_double)(*z)
        Tr = ct.c_double()
        rhor = ct.c_double()
        self._REDXdll(z, ct.byref(Tr), ct.byref(rhor))
        return Tr.value, rhor.value

    def SETPATHdll(self, hpth):
        hpth = ct.create_string_buffer(hpth.encode('utf-8'), 256)
        self._SETPATHdll(hpth, 255)

    def SETUPdll(self, nc, fld, hmx, ref):
        nc = ct.c_long(nc)
        hfld = ct.create_string_buffer(fld.encode('utf-8'), 10001)
        hhmx = ct.create_string_buffer(hmx.encode('utf-8'), 256)
        href = ct.create_string_buffer(ref.encode('utf-8'), 4)
        ierr = ct.c_long(0)
        herr = ct.create_string_buffer(255)

        self._SETUPdll(ct.byref(nc), hfld, hhmx, href, ct.byref(ierr), herr, 10000, 255, 3, 255)
        return ierr.value, trim(herr.raw)

    def NAMEdll(self, icomp):
        """ subroutine NAMEdll (icomp,hnam,hn80,hcasn) """
        icomp = ct.c_long(icomp)
        hnam = ct.create_string_buffer(12)
        hn80 = ct.create_string_buffer(80)
        hcasn = ct.create_string_buffer(12)
        self._NAMEdll(ct.byref(icomp), hnam, hn80, hcasn, 12, 80, 12)
        return str(hnam.raw), str(hn80.raw), hcasn.raw

    def GETMODdll(self, icomp, htype):
        """
        subroutine GETMODdll (icomp,htype,hcode,hcite)
        """
        icomp = ct.c_long(icomp)
        htype = ct.create_string_buffer(htype, 3)
        hcode = ct.create_string_buffer(3)
        hcite = ct.create_string_buffer(255)
        self._GETMODdll(ct.byref(icomp), htype, hcode, hcite, 3, 3, 255)
        return str(hcite.raw), str(hcode.raw)

    def SETREFdll(self,hrf,ixflag,x0,h0,s0,T0,P0):
        """
        subroutine SETREF (hrf,ixflag,x0,h0,s0,T0,P0,ierr,herr)
        """
        hrf = ct.create_string_buffer(hrf, 3)
        ixflag = ct.c_long(ixflag)
        x0 = (len(x0)*ct.c_double)(*x0)
        h0 = ct.c_double(h0)
        s0 = ct.c_double(s0)
        T0 = ct.c_double(T0)
        P0 = ct.c_double(P0)

        ierr = ct.c_long(0)
        herr = ct.create_string_buffer(255)

        self._SETREFdll(hrf, ct.byref(ixflag), x0, ct.byref(h0), ct.byref(s0),ct.byref(T0),ct.byref(P0),ct.byref(ierr), herr, 3, 255)
        return ierr.value, trim(herr.raw)

    def LIMITSdll(self, typ, z):
        htyp = ct.create_string_buffer(typ, 4)
        Tmin, Tmax, Dmax, Pmax = ct.c_double(),ct.c_double(),ct.c_double(),ct.c_double()
        z = (len(z)*ct.c_double)(*z)

        self._LIMITSdll(htyp, z, ct.byref(Tmin), ct.byref(Tmax), ct.byref(Dmax), ct.byref(Pmax), 3)
        return Tmin.value, Tmax.value, Dmax.value, Pmax.value

    def INFOdll(self, icomp):
        """ subroutine INFOdll (icomp,wmm,Ttrp,Tnbpt,Tc,Pc,Dc,Zc,acf,dip,Rgas) """
        icomp = ct.c_long(icomp)
        wmm, Ttrp, Tnbpt, Tc, Pc, Dc, Zc, acf, dip, Rgas = [ct.c_double(0) for _ in range(10)]
        self._INFOdll(ct.byref(icomp), ct.byref(wmm), ct.byref(Ttrp), ct.byref(Tnbpt), ct.byref(Tc), ct.byref(Pc), ct.byref(Dc), ct.byref(Zc), ct.byref(acf), ct.byref(dip), ct.byref(Rgas))
        return [a.value for a in [wmm, Ttrp, Tnbpt, Tc, Pc, Dc, Zc, acf, dip, Rgas]]

    def WMOLdll(self, z):
        z = (len(z)*ct.c_double)(*z)
        wm = ct.c_double()
        self._WMOLdll(z, ct.byref(wm))
        return wm.value

    def RMIX2dll(self, z):
        z = (len(z)*ct.c_double)(*z)
        R = ct.c_double()
        self._RMIX2dll(z, ct.byref(R))
        return R.value

    def MELTTdll(self, T, z):
        T = ct.c_double(T)
        z = (len(z)*ct.c_double)(*z)
        Pmelt = ct.c_double()
        ierr = ct.c_long(0)
        herr = ct.create_string_buffer(255)

        self._MELTTdll(ct.byref(T), z, ct.byref(Pmelt), ct.byref(ierr), herr, 255)
        return Pmelt.value, ierr.value, trim(herr.raw)

    def CRTPNTdll(self, z, Tc, pc, rhoc_mol_L):
        z = (len(z)*ct.c_double)(*z)
        Tc = ct.c_double(Tc)
        Pc = ct.c_double(pc)
        Dc = ct.c_double(rhoc_mol_L)
        ierr = ct.c_long(0)
        herr = ct.create_string_buffer(255)

        self._CRTPNTdll(z, ct.byref(Tc), ct.byref(Pc), ct.byref(Dc), ct.byref(ierr), herr, 255)
        return Tc.value, Pc.value, Dc.value, ierr.value, trim(herr.raw)

    def TRNPRPdll(self,T,D,z):
        """ 
        subroutine TRNPRPdll (T,D,z,eta,tcx,ierr,herr)
        """ 
        T = ct.c_double(T)
        D = ct.c_double(D)
        z = (len(z)*ct.c_double)(*z)
        eta = ct.c_double()
        tcx = ct.c_double()
        ierr = ct.c_long(0)
        herr = ct.create_string_buffer(255)

        self._TRNPRPdll(ct.byref(T), ct.byref(D), z, ct.byref(eta), ct.byref(tcx), ct.byref(ierr), herr, 255)
        return eta.value, tcx.value, ierr.value, trim(herr.raw)

    def SURFTdll(self,T,D,z):
        """ 
        subroutine SURFTdll (T,Dl,z,sigma,ierr,herr)
        """ 
        T = ct.c_double(T)
        D = ct.c_double(D)
        z = (len(z)*ct.c_double)(*z)
        sigma = ct.c_double()
        ierr = ct.c_long(0)
        herr = ct.create_string_buffer(255)

        self._SURFTdll(ct.byref(T), ct.byref(D), z, ct.byref(sigma), ct.byref(ierr), herr, 255)
        return sigma.value, ierr.value, trim(herr.raw)

    def CRITPdll(self, z, Tc, pc, rhoc_mol_L):
        z = (len(z)*ct.c_double)(*z)
        Tc = ct.c_double(Tc)
        Pc = ct.c_double(pc)
        Dc = ct.c_double(rhoc_mol_L)
        ierr = ct.c_long(0)
        herr = ct.create_string_buffer(255)

        self._CRITPdll(z, ct.byref(Tc), ct.byref(Pc), ct.byref(Dc), ct.byref(ierr), herr, 255)
        return Tc.value, Pc.value, Dc.value, ierr.value, trim(herr.raw)


    def RESIDUALdll(self, T, rho_mol_L, z):
        """ subroutine RESIDUALdll (T,D,z,Pr,er,hr,sr,Cvr,Cpr,ar,gr) """
        pr, er, hr, sr,Cvr,Cpr,ar,gr = ct.c_double(),ct.c_double(),ct.c_double(),ct.c_double(),ct.c_double(),ct.c_double(),ct.c_double(),ct.c_double()
        T = ct.c_double(T)
        rho_mol_L = ct.c_double(rho_mol_L)
        z = (len(z)*ct.c_double)(*z)

        self._RESIDUALdll(ct.byref(T), ct.byref(rho_mol_L), z, 
                       ct.byref(pr), ct.byref(er), ct.byref(hr), ct.byref(sr), ct.byref(Cvr), ct.byref(Cpr), ct.byref(ar), ct.byref(gr))
        return pr.value, er.value, hr.value, sr.value, Cvr.value, Cpr.value, ar.value, gr.value


    def THERMdll(self, T, rho_mol_L, z):
        """ subroutine THERMdll (T,D,z,P,e,h,s,Cv,Cp,w,hjt) """

        p, e, h, s,Cv,Cp,w,hjt = ct.c_double(),ct.c_double(),ct.c_double(),ct.c_double(),ct.c_double(),ct.c_double(),ct.c_double(),ct.c_double()
        T = ct.c_double(T)
        rho_mol_L = ct.c_double(rho_mol_L)
        z = (len(z)*ct.c_double)(*z)

        self._THERMdll(ct.byref(T), ct.byref(rho_mol_L), z, 
                       ct.byref(p), ct.byref(e), ct.byref(h), ct.byref(s), ct.byref(Cv), ct.byref(Cp), ct.byref(w), ct.byref(hjt))
        return p.value, e.value, h.value, s.value, Cv.value, Cp.value, w.value, hjt.value

    def TDFLSHdll(self, T, rho_mol_L, z):
        p, Dl, Dv, q, e, h, s, Cv, Cp, w = ct.c_double(),ct.c_double(),ct.c_double(),ct.c_double(),ct.c_double(),ct.c_double(),ct.c_double(),ct.c_double(),ct.c_double(),ct.c_double()
        T = ct.c_double(T)
        rho_mol_L = ct.c_double(rho_mol_L)
        z = (len(z)*ct.c_double)(*z)
        x = (len(z)*ct.c_double)()
        y = (len(z)*ct.c_double)()
        ierr = ct.c_long(0)
        herr = ct.create_string_buffer(255)

        self._TDFLSHdll(ct.byref(T), ct.byref(rho_mol_L), z, 
                        ct.byref(p), ct.byref(Dl), ct.byref(Dv), x, y, 
                        ct.byref(q), ct.byref(e), ct.byref(h), ct.byref(s), ct.byref(Cv), ct.byref(Cp), ct.byref(w), 
                        ct.byref(ierr), herr, 255)
        return p.value, Dl.value, Dv.value, list(x), list(y), q.value, e.value, h.value, s.value, Cv.value, Cp.value, w.value, ierr.value, trim(herr.raw)

    def TPFLSHdll(self, T, p, z):
        """
        subroutine TPFLSH (T,P,z,D,Dl,Dv,x,y,q,e,h,s,Cv,Cp,w,ierr,herr)
        """
        D, Dl, Dv, q, e, h, s, Cv, Cp, w = ct.c_double(),ct.c_double(),ct.c_double(),ct.c_double(),ct.c_double(),ct.c_double(),ct.c_double(),ct.c_double(),ct.c_double(),ct.c_double()
        T = ct.c_double(T)
        p = ct.c_double(p)
        z = (len(z)*ct.c_double)(*z)
        x = (len(z)*ct.c_double)()
        y = (len(z)*ct.c_double)()
        ierr = ct.c_long(0)
        herr = ct.create_string_buffer(255)

        self._TPFLSHdll(ct.byref(T), ct.byref(p), z, 
                        ct.byref(D), ct.byref(Dl), ct.byref(Dv), x, y, 
                        ct.byref(q), ct.byref(e), ct.byref(h), ct.byref(s), ct.byref(Cv), ct.byref(Cp), ct.byref(w), 
                        ct.byref(ierr), herr, 255)
        return D.value, Dl.value, Dv.value, list(x), list(y), q.value, e.value, h.value, s.value, Cv.value, Cp.value, w.value, ierr.value, trim(herr.raw)

    def PEFLSHdll(self, p, u_J_mol, z):
        D, T, Dl, Dv, q, e, h, s, Cv, Cp, w = ct.c_double(),ct.c_double(),ct.c_double(),ct.c_double(),ct.c_double(),ct.c_double(),ct.c_double(),ct.c_double(),ct.c_double(),ct.c_double(),ct.c_double()
        p = ct.c_double(p)
        u_J_mol = ct.c_double(u_J_mol)
        z = (len(z)*ct.c_double)(*z)
        x = (len(z)*ct.c_double)()
        y = (len(z)*ct.c_double)()
        ierr = ct.c_long(0)
        herr = ct.create_string_buffer(255)

        self._PEFLSHdll(ct.byref(p), ct.byref(u_J_mol), z, ct.byref(T), ct.byref(D), 
                        ct.byref(Dl), ct.byref(Dv), x, y, 
                        ct.byref(q), ct.byref(h), ct.byref(s), ct.byref(Cv), ct.byref(Cp), ct.byref(w), 
                        ct.byref(ierr), herr, 255)
        return T.value, D.value, Dl.value, Dv.value, list(x), list(y), q.value, h.value, s.value, Cv.value, Cp.value, w.value, ierr.value, trim(herr.raw)

    def SATTdll(self, T, z, q):
        T = ct.c_double(T)
        z = (len(z)*ct.c_double)(*z)
        if (abs(q) < 1e-10):
            kph = ct.c_long(1)
        elif (abs(q-1) < 1e-10):
            kph = ct.c_long(2)
        else:
            raise ValueError()
        p, dl, dv = ct.c_double(),ct.c_double(),ct.c_double()
        x = (len(z)*ct.c_double)()
        y = (len(z)*ct.c_double)()
        ierr = ct.c_long(0)
        herr = ct.create_string_buffer(255)

        # SATTdll (T,z,kph,P,Dl,Dv,x,y,ierr,herr)
        self._SATTdll(ct.byref(T), z, ct.byref(kph), ct.byref(p), 
                      ct.byref(dl), ct.byref(dv), x, y, 
                      ct.byref(ierr), herr, 255)
        return p, dl.value, dv.value, list(x), list(y), ierr.value, trim(herr.raw)

    def SATTPdll(self, T, p, z, kph, iguess, d):
        T = ct.c_double(T)
        p = ct.c_double(p)
        z = (len(z)*ct.c_double)(*z)
        kph = ct.c_long(kph)
        iguess = ct.c_long(iguess)
        d, dl, dv = ct.c_double(d), ct.c_double(), ct.c_double()
        x = (len(z)*ct.c_double)()
        y = (len(z)*ct.c_double)()
        q = ct.c_double()
        ierr = ct.c_long(0)
        herr = ct.create_string_buffer(255)

        # SATTPdll(t, p, z, kph, iguess, d, dl, dv, x, y, q, ierr, herr, herr_length)
        self._SATTPdll(ct.byref(T), ct.byref(p), z, ct.byref(kph), ct.byref(iguess), 
                       ct.byref(d), ct.byref(dl), ct.byref(dv), x, y, ct.byref(q),
                       ct.byref(ierr), herr, 255)

        return T.value, p.value, d.value, dl.value, dv.value, list(x), list(y), q.value, ierr.value, trim(herr.raw)

    def TQFLSHdll(self, T, q, z, kq = 1):
        T = ct.c_double(T)
        q = ct.c_double(q)
        z = (len(z)*ct.c_double)(*z)
        kq = ct.c_long(kq)
        p, d, dl, dv = ct.c_double(),ct.c_double(),ct.c_double(),ct.c_double()
        x = (len(z)*ct.c_double)()
        y = (len(z)*ct.c_double)()
        e, h, s, Cv, Cp, w  = ct.c_double(),ct.c_double(),ct.c_double(),ct.c_double(),ct.c_double(),ct.c_double()
        ierr = ct.c_long(0)
        herr = ct.create_string_buffer(255)

        # TQFLSHdll(T,q,z,kq,P,D,Dl,Dv,x,y,e,h,s,Cv,Cp,w,i,herr)
        self._TQFLSHdll(ct.byref(T), ct.byref(q), z, ct.byref(kq),
                        ct.byref(p), ct.byref(d), ct.byref(dl), ct.byref(dv), x, y, ct.byref(e), ct.byref(h), ct.byref(s), ct.byref(Cv), ct.byref(Cp), ct.byref(w), 
                        ct.byref(ierr), herr, 255)
        return p.value, d.value, dl.value, dv.value, list(x), list(y), e.value, h.value, s.value, Cv.value, Cp.value, w.value, ierr.value, trim(herr.raw)

    def TPRHOdll(self, T,P,z,kph,kguess,D = 0):
        """ subroutine TPRHOdll (T,P,z,kph,kguess,D,ierr,herr) """
        T = ct.c_double(T)
        P = ct.c_double(P)
        z = (len(z)*ct.c_double)(*z)
        kph = ct.c_long(kph)
        kguess = ct.c_long(kguess)
        D = ct.c_double(D)
        ierr = ct.c_long(0)
        herr = ct.create_string_buffer(255)
        self._TPRHOdll(ct.byref(T), ct.byref(P), z, ct.byref(kph), ct.byref(kguess), ct.byref(D), 
                       ct.byref(ierr), herr, 255)
        return D.value, ierr.value, trim(herr.raw)

