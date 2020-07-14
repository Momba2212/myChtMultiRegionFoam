    const wordList fluidNames(rp["fluid"]);                 // reginProperties rp      defined earlier, reads the regionPropertiesDict    fluid   (air porous)
    
    PtrList<fvMesh> fluidRegions(fluidNames.size());        //  PtrList is a container for pointers to objects and there is a bunch of templates in it
    
    // creating the fluid mesh for the regions within the fluid-directory
    
    forAll(fluidNames, i)
    {
        Info<< "Create fluid mesh for region " << fluidNames[i]
            << " for time = " << runTime.timeName() << nl << endl;
            
        fluidRegions.set        // container werden immer durch einen Zeiger repräsentiert, welcher auf die Elemente zeigt
        (
            i,
            new fvMesh          // deshalb new
            (
                IOobject
                (
                    fluidNames[i],          // name des Felds
                    runTime.timeName(),     // zeigt die Zeit
                    runTime,
                    IOobject::MUST_READ
                )
            )
        );
    }
        
            
