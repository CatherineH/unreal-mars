// UE4 Procedural Mesh Generation from the Epic Wiki (https://wiki.unrealengine.com/Procedural_Mesh_Generation)

#pragma once

#include "GameFramework/Actor.h"
#include "ProceduralMeshComponent.h"
#include "Networking.h"
#include "Sockets.h"
#include "SocketSubsystem.h"
//#include "Runtime/Sockets/Public/SocketSubsystem.h"
//#include "Runtime/Networking/Public/Interfaces/IPv4/IPv4Endpoint.h"
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

	void GenerateLandscape(TArray<uint8> InPoints, const int xdim, const int ydim, const int grid_spacing, TArray<FProceduralMeshTriangle>& OutTriangles);
	TArray<uint8> GetTile(const FString SocketName, const FString& ip, const int32 port, const int x_loc, const int y_loc);

};
