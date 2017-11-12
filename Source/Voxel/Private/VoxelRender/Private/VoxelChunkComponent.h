// Copyright 2017 Phyronnaz

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "VoxelProceduralMeshComponent.h"
#include "TransitionDirection.h"
#include "LandscapeGrassType.h"
#include "VoxelThread.h"
#include "VoxelChunkComponent.generated.h"

class FVoxelRender;
class FChunkOctree;
class FVoxelPolygonizer;

/**
 * Voxel Chunk actor class
 */
UCLASS()
class UVoxelChunkComponent : public UVoxelProceduralMeshComponent
{
	GENERATED_BODY()

public:
	UVoxelChunkComponent();
	~UVoxelChunkComponent();

	/**
	 * Init this
	 * @param	NewPosition		Position of this (Minimal corner)
	 * @param	NewDepth		Width = 16 * 2^Depth
	 * @param	NewWorld		VoxelWorld
	 */
	void Init(TWeakPtr<FChunkOctree> NewOctree);

	/**
	 * Update this for terrain changes
	 * @param	bAsync
	 */
	bool Update(bool bAsync);

	bool UpdateFoliage();

	/**
	 * Check if an adjacent chunk has changed its resolution, and update async if needed
	 */
	void CheckTransitions();

	/**
	 * Schedule destruction of this chunk
	 */
	void Unload();

	void Delete();

	void SetVoxelMaterial(UMaterialInterface* Material);

	bool HasChunkHigherRes(TransitionDirection Direction);

	/**
	* Copy Task section to PrimaryMesh section
	*/
	void OnMeshComplete(FVoxelProcMeshSection& InSection);

	void ApplyNewMesh();

	void OnFoliageComplete();

	void ApplyNewFoliage();

	void ResetRender();

protected:
	void Serialize(FArchive& Ar) override;

private:
	TArray<UHierarchicalInstancedStaticMeshComponent*> FoliageComponents;

	FVoxelProcMeshSection Section;

	FTimerHandle DeleteTimer;

	// ChunkHasHigherRes[TransitionDirection] if Depth != 0
	TArray<bool, TFixedAllocator<6>> ChunkHasHigherRes;

	// Async process tasks
	FAsyncTask<FAsyncPolygonizerTask>* MeshBuilder;
	TArray<FAsyncTask<FAsyncFoliageTask>*> FoliageTasks;

	FVoxelPolygonizer* Builder;

	TSharedPtr<FChunkOctree> CurrentOctree;
	FVoxelRender* Render;

	FThreadSafeCounter CompletedFoliageTaskCount;

	void OnAllFoliageComplete();

	void DeleteTasks();

	void CreateBuilder();
};