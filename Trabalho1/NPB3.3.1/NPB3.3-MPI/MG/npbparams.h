c NPROCS = 64 CLASS = B
c  
c  
c  This file is generated automatically by the setparams utility.
c  It sets the number of processors and the class of the NPB
c  in this directory. Do not modify it by hand.
c  
        integer nprocs_compiled
        parameter (nprocs_compiled = 64)
        integer nx_default, ny_default, nz_default
        parameter (nx_default=256, ny_default=256, nz_default=256)
        integer nit_default, lm, lt_default
        parameter (nit_default=20, lm = 6, lt_default=8)
        integer debug_default
        parameter (debug_default=0)
        integer ndim1, ndim2, ndim3
        parameter (ndim1 = 6, ndim2 = 6, ndim3 = 6)
        logical  convertdouble
        parameter (convertdouble = .false.)
        character*11 compiletime
        parameter (compiletime='20 Apr 2020')
        character*5 npbversion
        parameter (npbversion='3.3.1')
        character*6 cs1
        parameter (cs1='mpif90')
        character*9 cs2
        parameter (cs2='$(MPIF77)')
        character*5 cs3
        parameter (cs3='-lmpi')
        character*6 cs4
        parameter (cs4='(none)')
        character*3 cs5
        parameter (cs5='-O3')
        character*3 cs6
        parameter (cs6='-O3')
        character*6 cs7
        parameter (cs7='randi8')
