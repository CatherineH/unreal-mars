// UE4 Procedural Mesh Generation from the Epic Wiki (https://wiki.unrealengine.com/Procedural_Mesh_Generation)

#pragma once

#include "GameFramework/Actor.h"
#include "ProceduralMeshComponent.h"
#include "ProceduralLandscape.generated.h"

/**
*
*/
UCLASS()
class PROCEDURALMESH_API AProceduralLandscape : public AActor
{
	GENERATED_BODY()

public:
	AProceduralLandscape();

	// Allow viewing/changing the Material ot the procedural Mesh in editor (if placed in a level at construction)
	UPROPERTY(VisibleAnywhere, Category = Materials)
		UProceduralMeshComponent* mesh;

	void GenerateLandscape(int InPoints[4][4], const int xdim, const int ydim, const int grid_spacing, TArray<FProceduralMeshTriangle>& OutTriangles);
};
