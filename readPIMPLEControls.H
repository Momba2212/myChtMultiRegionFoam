    // We do not have a top-level mesh. Construct the fvSolution for the runTime instead
    fvSolution solutionDict(runTime);
    
    const dictionary& pimple = solutionDict.subDict("PIMPLE");                              // so pimple is the address of the pimple-dictionary { nCorr 5; ... }
    
    const int nOuterCorr = pimple.lookOrDefault<int>("nOuterCorrectors", 1);                // es ist kein IOdictionary!  Hier suche ich in der Addresse des dictionaries nach dem Eintrag oder setze es auf 1
