dictionary& pimple = mesh.solutionDict().subDict("PIMPLE");

const int nNonOrthCorr = 
    pimple.lookupOrDefault<int>("nNonOrthCorrectors", 0);
