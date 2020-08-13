// I don't need a header-guard, since every I need, was included before

const wordList solidNames(rp["solid"]);                              // regionProperties rp(runTime);   >> read out from regionPropertiesDict   solid (holz stein)

PtrList<fvMesh> solidRegions(solidNames.size());                      // Liste mit solidNames.size() Plätzen (bspw. 2 , da holz und stein)


// define the meshes for holz and stein
forAll(solidNames, i)
{
        Info<< "Create solid mesh for region " << solidNames[i]
            << " for time = " << runTime.timeName() << nl << endl;
    // befülle Liste solidRegions mit der set-function
    solidRegions.set
    (
        i,                                                          // an der ersten Stelle ist das erste mesh der region solid (hier von holz)                                                                     
        new fvMesh
        (
            IOobject
            (
                fluidNames[i],                                      // name of the mesh
                runTime.timeName(),
                runTime,
                IOobject::MUST_READ
            )
        )
    );
}
             
