/*---------------------------------------------------------------------------*\
  =========                 |
  \\      /  F ield         | OpenFOAM: The Open Source CFD Toolbox
   \\    /   O peration     |
    \\  /    A nd           | www.openfoam.com
     \\/     M anipulation  |
-------------------------------------------------------------------------------
    Copyright (C) 2020 AUTHOR,AFFILIATION
-------------------------------------------------------------------------------
License
    This file is part of OpenFOAM.

    OpenFOAM is free software: you can redistribute it and/or modify it
    under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    OpenFOAM is distributed in the hope that it will be useful, but WITHOUT
    ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or
    FITNESS FOR A PARTICULAR PURPOSE.  See the GNU General Public License
    for more details.

    You should have received a copy of the GNU General Public License
    along with OpenFOAM.  If not, see <http://www.gnu.org/licenses/>.

    
Application
    myCHTMultiRegionsFoam

Group
    grpHeatTransferSolvers

Description
    Transient solver for buoyant, turbulent fluid flow and solid heat
    conduction with conjugate heat transfer between solid and fluid regions.

    It handles secondary fluid or solid circuits which can be coupled
    thermally with the main fluid region. i.e radiators, etc.
\*---------------------------------------------------------------------------*/


#include "fvCFD.H"


#include "regionProperties.H"       // creates the IOdictionary object, which reads out createRegionProperties





// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

int main (int argc, char* argv[])               // argv[0] = ./executionName argv[1]=firstFlag
{
    argList::addNote
    (
        "Transient solver for buoyant, turbulent fluid flow and solid heat"
        " conduction with conjugate heat transfer"
        " between solid and fluid regions."
    );
    
    #define NO_CONTROL
  #define CREATE_MESH createMeshesPostProcess.H
    #include "postProcess.H"    
    
    #include "setRootCaseList.H"             // This is setRootCase, but with additional solver-related listing
    #include "createTime.H"
    #include "createMeshes.H"               //  create fvMesh für die aus regionProperties.H gelesenen Regions!             
                                            // regionPropertiesDict    fluid   (air porous)
                                            // regionPropertiesDict    solid   (stein holz)
    #include "createFields.H"               // instanziiert die Größen auf dem Gitter

    #include "initContinuityErrs.H"         // ???
    #include "createTimeControls.H"         // global readTimeControls from the controlDict and sets the maxCo (Corount number)
    #include "readSolidTimeControls.H"      // Read the control parameters used in the solid    maxDi (diffusion number = stabilize the calculation like the Courant number)
    #include "compressibleMultiRegionCourantNo.H"       // global: setzt die Courant number für alle anderen Fluid regions
    #include "solidRegionDiffusionNo.H"                 // global: sets the diffusion coefficients for all solid regions
    #include "setInitialMultiRegionDeltaT.H"
    
    
    // The actual solving algorithm looks like this
    
    while (runTime.run())
    {
        #include "readTimeControls.H"          // Read the control parameters used by setDeltaT 
//        #include "readSolidTimeControls.H"     // nochmal maxDi definieren > ist nicht nötig
        #include "readPIMPLEControls.H"         // reads the fvSolutionDict for the pimple settings:   dictionary& pimple
        
//        #include "compressibleMultiRegionCourantNo.H"   // global: sets the courant number (Again)
//        #include "solidRegionDiffusionNo.H"               // global: sets the diffusion number for the solid region
        #include "setMultiRegionDeltaT.H"       // global: Reset the timestep to maintain a constant maximum Courant numbers.
                                                //Reduction of time-step is immediate, but increase is damped to avoid unstable oscillations
    
    ++runTime;                                  // begin with the first timeStep
    
    Info << "Time = " << runTime.timeName() << nl << endl;
    
    if (nOuterCorr != 1)
    {
        forAll(fluidRegiuons,  i)
        {
            #include "storeOldFluidFields.H"       //  p_rghFluid[i].storePrevIter();
        }
    }
    
    
    // -- PIMPLE loop
    for (int oCorr=0; oCorr<nOuterCorr; ++oCorr)
    {
        const bool finalIter = (oCorr == nOuterCorr-1);     // sets finalIter to 1, if final iteration loop
        
        // Solve fluid regions
        forAll(fluidRegions, i)                                // fluidRegions ist eine Liste (genau einePtrList)
        {
            Info<< "\nSolving for fluid region " << fluidRegions[i].name() << endl; 
            
            #include "setRegionFluidFields.H"           // gelöst wird immer auf dem mesh  fvMesh& mesh = fluidRegions[i];  >> muss hier die Fields von den 
            #include "readFluidMultiRegionPIMPLEControls.H" // 
            #include "solveFluid.H"
        }
        
        
        // Solve solid regions
        forAll(solidRegions, i)
        {
            Info<< "Solving for solid region " << solidRegions[i] << endl;
            
            #include "setRegionSolidFields.H"   
            #include "readSolidMultiRegionPIMPLEControls.H"
            #include "solveSolid.H"
        }
        
        // Additional loops for energy solution only 
        if (!oCorr && nOuterCorr > 1)
        {
                loopControl looping(runTime, pimple, "energyCoupling");

                while (looping.loop())
                {
                    Info<< nl << looping << nl;

                    forAll(fluidRegions, i)
                    {
                        Info<< "\nSolving for fluid region "
                            << fluidRegions[i].name() << endl;
                       #include "setRegionFluidFields.H"
                       #include "readFluidMultiRegionPIMPLEControls.H"
                       frozenFlow = true;
                       #include "solveFluid.H"
                    }

                    forAll(solidRegions, i)
                    {
                        Info<< "\nSolving for solid region "
                            << solidRegions[i].name() << endl;
                        #include "setRegionSolidFields.H"
                        #include "readSolidMultiRegionPIMPLEControls.H"
                        #include "solveSolid.H"
                    }
                }
            }
        }   
        
        runTime.write();
        
        runTime.printExecutionTime(Info);
    }
    
    
    // Changes for Git
    
    int a = 5;
    if(a==5)
    {
        Info<< "Hallo, a ist gleich 5 " << endl;
    }
    
    Info<< "End\n" << endl;
    
    return 0;
    
    
}
    
    
    




// ************************************************************************* //
