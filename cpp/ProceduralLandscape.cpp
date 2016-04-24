// UE4 Procedural Mesh Generation from the Epic Wiki (https://wiki.unrealengine.com/Procedural_Mesh_Generation)
//
// A landscape object
#include "ProceduralMesh.h"
#include "ProceduralLandscape.h"
#define BUFSIZE 4096


AProceduralLandscape::AProceduralLandscape()
{
	mesh = CreateDefaultSubobject<UProceduralMeshComponent>(TEXT("ProceduralLandscape"));
	
	// Apply a material
	static ConstructorHelpers::FObjectFinder<UMaterialInterface> Material(TEXT("/Game/Materials/BaseColor.BaseColor"));
	mesh->SetMaterial(0, Material.Object);
	TArray<uint8> in_data = GetTile("proc_landscape", "127.0.0.1", 8890, 1000, 7000, FString("points"));
	TArray<uint8> in_r;// = GetTile("proc_landscape", "127.0.0.1", 8890, 1000, 7000, FString("r"));
	TArray<uint8> in_g;// = GetTile("proc_landscape", "127.0.0.1", 8890, 1000, 7000, FString("g"));
	TArray<uint8> in_b;// = GetTile("proc_landscape", "127.0.0.1", 8890, 1000, 7000, FString("b"));
	const int grid_size = 256;
	/*
	in_r[0] = 0;
	while (in_r.Num() < grid_size*grid_size)
	{		
		in_r.Add(in_r.Last());
	}
	in_g[0] = 0;
	while (in_g.Num() < grid_size*grid_size)
	{
		in_g.Add(in_g.Last());
	}
	in_b[0] = 0;
	while (in_b.Num() < grid_size*grid_size)
	{
		in_b.Add(in_b.Last());
	}
	*/

	// Contains an array of landscape heights
	TArray<uint8> points = Decompress(in_data, grid_size);
	/*
	TArray<uint8> data_r = Decompress(in_r, grid_size);
	TArray<uint8> data_g = Decompress(in_g, grid_size);
	TArray<uint8> data_b = Decompress(in_b, grid_size);*/
	int curr_x = 0;
	int curr_y = 0;
	
	// Generate a Lathe from rotating the given points
	TArray<FProceduralMeshTriangle> triangles;
	int scaling_factor = 10120;
	GenerateLandscape(points, in_r, in_g, in_b, grid_size, grid_size, scaling_factor, triangles);
	mesh->SetProceduralMeshTriangles(triangles);

	RootComponent = mesh;
}

// Generate a lathe by rotating the given polyline
void AProceduralLandscape::GenerateLandscape(TArray<uint8> InPoints, TArray<uint8> InR, TArray<uint8> InG, TArray<uint8> InB, const int xdim, const int ydim, const int grid_spacing, TArray<FProceduralMeshTriangle>& OutTriangles)
{
	UE_LOG(LogClass, Log, TEXT("AProceduralLandscape::Landscape DIMS %d %d GRIDSPACING %d"), xdim, ydim, grid_spacing);
	FProceduralMeshTriangle tri;
	FColor Red(255, 0, 0);
	tri.Vertex0.Color = Red;
	tri.Vertex1.Color = Red;
	tri.Vertex2.Color = Red;
	// for each position in the grid, draw the lower triangle and then the upper triangle
	for (int x_index = 0; x_index<xdim-1; x_index++)
	{
		for (int y_index = 0; y_index<ydim-1; y_index++)
		{
			// lower triangle
			FVector point_vector1(x_index*grid_spacing, y_index*grid_spacing, InPoints[x_index+y_index*xdim]*grid_spacing);
			tri.Vertex0.Position = point_vector1;
			//FColor Vert_col1(InR[x_index + y_index*xdim], InG[x_index + y_index*xdim], InB[x_index + y_index*xdim]);
			//tri.Vertex0.Color = Vert_col1;

			FVector point_vector2(x_index*grid_spacing, (y_index+1)*grid_spacing, InPoints[x_index+(y_index+1)*xdim]*grid_spacing);
			tri.Vertex1.Position = point_vector2;
			//FColor Vert_col2(InR[x_index + (y_index + 1)*xdim], InG[x_index + (y_index + 1)*xdim], InB[x_index + (y_index + 1)*xdim]);
			//tri.Vertex1.Color = Vert_col2;

			FVector point_vector3((x_index+1)*grid_spacing, y_index*grid_spacing, InPoints[x_index+1+y_index*xdim]*grid_spacing);
			tri.Vertex2.Position = point_vector3;
			//FColor Vert_col3(InR[x_index + 1 + y_index*xdim], InG[x_index + 1 + y_index*xdim], InB[x_index + 1 + y_index*xdim]);
			//tri.Vertex2.Color = Vert_col3;
			OutTriangles.Add(tri);


			// upper triangle
			FVector point_vector4(x_index*grid_spacing, (y_index+1)*grid_spacing, InPoints[x_index+(y_index+1)*xdim]*grid_spacing);
			tri.Vertex0.Position = point_vector4;
			//FColor Vert_col4(InR[x_index + (y_index + 1)*xdim], InG[x_index + (y_index + 1)*xdim], InB[x_index + (y_index + 1)*xdim]);
			//tri.Vertex0.Color = Vert_col4;
			FVector point_vector5((x_index+1)*grid_spacing, (y_index+1)*grid_spacing, InPoints[x_index+1+(y_index+1)*xdim]*grid_spacing);
			tri.Vertex1.Position = point_vector5;
			//FColor Vert_col5(InR[x_index + 1 + (y_index + 1)*xdim], InG[x_index + 1 + (y_index + 1)*xdim], InB[x_index + 1 + (y_index + 1)*xdim]);
			//tri.Vertex1.Color = Vert_col5;
			FVector point_vector6((x_index+1)*grid_spacing, y_index*grid_spacing, InPoints[x_index+1+y_index*xdim]*grid_spacing);
			tri.Vertex2.Position = point_vector6;
			//FColor Vert_col6(InR[x_index + 1 + y_index*xdim], InG[x_index + 1 + y_index*xdim], InB[x_index + 1 + y_index*xdim]);
			//tri.Vertex2.Color = Vert_col6;
			OutTriangles.Add(tri);
		}
	}
	
}


TArray<uint8> AProceduralLandscape::GetTile(const FString SocketName, const FString& ip, const int32 port, const int x_loc, const int y_loc, FString _data_type)
{
	TArray<uint8> ReceivedData;
	TSharedRef<FInternetAddr> RemoteAddress = ISocketSubsystem::Get(PLATFORM_SOCKETSUBSYSTEM)->CreateInternetAddr();
	
	TSharedRef<FInternetAddr> ReceivedAddr = ISocketSubsystem::Get(PLATFORM_SOCKETSUBSYSTEM)->CreateInternetAddr();
	
	int32 BufferSize = 65536;
	int32 sent = 0;
	FString message = FString::Printf(TEXT("%s_%d_%d"), *_data_type, x_loc, y_loc);
	bool bIsValid;
	RemoteAddress->SetIp(*ip, bIsValid);
	RemoteAddress->SetPort(port);
	

	if (!bIsValid)
	{
		UE_LOG(LogClass, Log, TEXT("IP Not Valid"));
		return ReceivedData;
	}
	FSocket* TCPSocket = FTcpSocketBuilder("SOCKETNAME").AsNonBlocking().AsReusable().Build();


	UE_LOG(LogTemp, Warning, TEXT("Socket created! Connecting to server..."));
	bool didConnect = TCPSocket->Connect(*RemoteAddress);
	if (didConnect)
	{
		UE_LOG(LogTemp, Warning, TEXT("Connection state: true"));
		bool successful = TCPSocket->SendTo((uint8*)TCHAR_TO_UTF8(*message), sizeof(message), sent, *RemoteAddress);
		if (!successful)
		{
			UE_LOG(LogClass, Log, TEXT("Send Not Successful"));
			TCPSocket->Close();
			return ReceivedData;
		}
		else
		{
			UE_LOG(LogClass, Log, TEXT("Send Successful"));
		}
	}
	else
	{
		UE_LOG(LogTemp, Log, TEXT("Connection state: false"));
	}
	

	uint32 Size = 65536;
	bool bIsDataReceived = false;
	
	while (!bIsDataReceived)
	{
		if (TCPSocket->HasPendingData(Size))
		{
			ReceivedData.Init(Size);

			int32 Read = 0;
			TCPSocket->RecvFrom(ReceivedData.GetData(), BufferSize, Read, *ReceivedAddr);

			if (Read > 0)
			{
				bIsDataReceived = true;
			}
		}
	}
	UE_LOG(LogTemp, Log, TEXT("got array of size %d"), ReceivedData.Num());
	TCPSocket->Close();
	
	return ReceivedData;
}

TArray<uint8> AProceduralLandscape::Decompress(TArray<uint8> in_data, const int32 grid_size)
{
	TArray<uint8> out_data;
	// copy over the served landscape heights to the points array
	for (int in_data_index = 0; in_data_index < in_data.Num(); in_data_index += 2)
	{
		for (int in_fill = 0; in_fill < in_data[in_data_index]; in_fill++)
		{
			out_data.Add(60 - in_data[in_data_index + 1]);
		}
	}
	// fill the rest with zeros
	while (in_data.Num() < grid_size*grid_size)
	{
		out_data.Add(60 - in_data.Last());
	}
	return out_data;
}