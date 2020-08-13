if (finalIter)
{
    mesh.data::add("finalIter", true);
}

if (frozenFlow)
{
    #include "EEeqn.H"
}
else
{
    if(oCorr == 0)
    {
        #include "rhoEqn.H"
    }
        
    #include "UEqn.H"
    #include "YEqn.H"
    #include "EEqn.H"    
    
    for (int corr = 0; corr<nCorr; cor++)
    {
        #include "pEqn.H"
    }
    
    turbulence.correct();
    
    rho = thermo.rho();
}
        
