    // mesh is now the regionMesh, like I defined in setFluidRegionFields
    const dictionary& pimple = mesh.solutionDict().subDict("PIMPLE");
    
    const int nCorr = 
        pimple.lookupOrDefault<int>("nCorrectors", 1);
        
    const int nNonOrthCorr = 
        pimple.lookupOrDefault<int>("nNonOrthCorrectors", 1)
        
    const bool momentumPredictor =
        pimple.lookupOrDefault<word>("momentumPredictor", true);
