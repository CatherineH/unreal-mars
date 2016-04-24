// UE4 Procedural Mesh Generation from the Epic Wiki (https://wiki.unrealengine.com/Procedural_Mesh_Generation)
//
// A landscape object
#include "ProceduralMesh.h"
#include "ProceduralLandscape.h"
#using namespace System;
#using namespace System::IO;
#define BUFSIZE 4096


AProceduralLandscape::AProceduralLandscape()
{
	mesh = CreateDefaultSubobject<UProceduralMeshComponent>(TEXT("ProceduralLandscape"));
	
	// Apply a material
	static ConstructorHelpers::FObjectFinder<UMaterialInterface> Material(TEXT("/Game/Materials/BaseColor.BaseColor"));
	mesh->SetMaterial(0, Material.Object);

	// Contains an array of landscape heights
	int points[4][4];
	points[0][0] = 0;
	points[1][0] = 0;
	points[2][0] = 0;
	points[3][0] = 0;
	
	points[0][1] = 0;
	points[1][1] = 0;
	points[2][1] = 1;
	points[3][1] = 0;
	
	points[0][2] = 0;
	points[1][2] = 0;
	points[2][2] = 0;
	points[3][2] = 0;
	
	points[0][3] = 0;
	points[1][3] = 0;
	points[2][3] = 1;
	points[3][3] = 0;
	
	
	// Generate a Lathe from rotating the given points
	TArray<FProceduralMeshTriangle> triangles;
	GenerateLandscape(points, 4, 4, 10, triangles);
	mesh->SetProceduralMeshTriangles(triangles);

	RootComponent = mesh;
}

// Generate a lathe by rotating the given polyline
void AProceduralLandscape::GenerateLandscape(int InPoints[4][4], const int xdim, const int ydim, const int grid_spacing, TArray<FProceduralMeshTriangle>& OutTriangles)
{
	String^ tile_cache = "textfile.txt";
	UE_LOG(LogClass, Log, TEXT("AProceduralLandscape::Landscape tile_cache %s"), tile_cache);
	UE_LOG(LogClass, Log, TEXT("AProceduralLandscape::Landscape DIMS %d %d"), xdim, ydim);
	FProceduralMeshTriangle tri;
	static const FColor Red(255, 51, 51);
	tri.Vertex0.Color = Red;
	tri.Vertex1.Color = Red;
	tri.Vertex2.Color = Red;
	// for each position in the grid, draw the lower triangle and then the upper triangle
	for (int x_index = 0; x_index<xdim-1; x_index++)
	{
		for (int y_index = 0; y_index<ydim-1; y_index++)
		{
			// lower triangle
			FVector point_vector1(x_index*grid_spacing, y_index*grid_spacing, InPoints[x_index][y_index]*grid_spacing);
			tri.Vertex0.Position = point_vector1;
			FVector point_vector2(x_index*grid_spacing, (y_index+1)*grid_spacing, InPoints[x_index][y_index+1]*grid_spacing);
			tri.Vertex1.Position = point_vector2;
			FVector point_vector3((x_index+1)*grid_spacing, y_index*grid_spacing, InPoints[x_index+1][y_index]*grid_spacing);
			tri.Vertex2.Position = point_vector3;
			OutTriangles.Add(tri);
			// upper triangle
			FVector point_vector4(x_index*grid_spacing, (y_index+1)*grid_spacing, InPoints[x_index][y_index+1]*grid_spacing);
			tri.Vertex0.Position = point_vector4;
			FVector point_vector5((x_index+1)*grid_spacing, (y_index+1)*grid_spacing, InPoints[x_index+1][y_index+1]*grid_spacing);
			tri.Vertex1.Position = point_vector5;
			FVector point_vector6((x_index+1)*grid_spacing, y_index*grid_spacing, InPoints[x_index+1][y_index]*grid_spacing);
			tri.Vertex2.Position = point_vector6;
			OutTriangles.Add(tri);
		}
	}
	
}