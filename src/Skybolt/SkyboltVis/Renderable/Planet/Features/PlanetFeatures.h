/* Copyright 2012-2020 Matthew Reid
 *
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at https://mozilla.org/MPL/2.0/. */

#pragma once

#include "SkyboltVis/SkyboltVisFwd.h"
#include "SkyboltVis/ShadowHelpers.h"
#include "SkyboltVis/Renderable/Planet/Features/PlanetFeaturesSource.h"
#include "SkyboltVis/Renderable/Water/LakesBatch.h"

#include <SkyboltCommon/File/FileUtility.h>
#include <SkyboltCommon/Math/QuadTree.h>
#include <px_sched/px_sched.h>
#include <osg/MatrixTransform>

namespace skybolt {
namespace vis {

struct VisFeatureTile : mapfeatures::FeatureTile
{
	std::unique_ptr<struct LoadedVisObjects> visObjects;
	bool loaded = false;
};

struct PlanetFeaturesParams
{
	static const int groupsBuildingsIndex = 0;
	static const int groupsNonBuildingsIndex = 1;
	static const int featureGroupsSize = 2;

	px_sched::Scheduler* scheduler;
	file::Path directory;
	const ShaderPrograms* programs;
	osg::ref_ptr<osg::StateSet> waterStateSet;
	double planetRadius;
	osg::Group* groups[featureGroupsSize];
	ShadowMaps shadowMaps;
};

class PlanetFeatures
{
public:
	PlanetFeatures(const PlanetFeaturesParams& params);
	~PlanetFeatures();

	void onSurfaceTileAdded(const skybolt::QuadTreeTileKey& key);

	void updatePreRender(const RenderContext& context);

private:
	void loadTile(VisFeatureTile& tile);
	void unloadTile(VisFeatureTile& tile);

private:
	void processLoadingQueue();

	void updatePreRender(LoadedVisObjects& objects, const RenderContext& context) const;
	void unload(LoadedVisObjects& objects) const;

private:
	const std::string mTileDirectory;
	px_sched::Scheduler* mScheduler;
	std::unique_ptr<class VisObjectsLoadTask> mVisObjectsLoadTask;

	osg::Group* mGroups[PlanetFeaturesParams::featureGroupsSize];
	const double mPlanetRadius;

	mapfeatures::WorldFeatures mFeatures;
	std::vector<LoadedVisObjects*> mLoadedVisObjects;

	struct LoadingItem
	{
		VisFeatureTile* tile;
		std::unique_ptr<LoadedVisObjects> objects;
		std::atomic<bool> cancel = false;
	};
	typedef std::shared_ptr<LoadingItem> LoadingItemPtr;

	std::vector<LoadingItemPtr> mLoadingQueue;
	px_sched::Sync mLoadingTaskSync;
};

} // namespace vis
} // namespace skybolt
