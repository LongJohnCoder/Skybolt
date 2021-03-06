/* Copyright 2012-2020 Matthew Reid
 *
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at https://mozilla.org/MPL/2.0/. */


#pragma once

#include "RenderTarget.h"
#include <osg/Texture2D>
#include <boost/optional.hpp>
#include <functional>

namespace skybolt {
namespace vis {

using TextureFactory = std::function<osg::ref_ptr<osg::Texture>(const osg::Vec2i&)>;

struct RenderTextureConfig
{
	TextureFactory colorTextureFactory;
	boost::optional<TextureFactory> depthTextureFactory;
	int multisampleSampleCount = 0;
};

class RenderTexture : public RenderTarget
{
public:
	RenderTexture(const RenderTextureConfig& config);
	~RenderTexture();

public: // RenderTarget interface
	void updatePreRender() override;

private:
	osg::ref_ptr<osg::Texture> mTexture;
	TextureFactory mColorTextureFactory;
	boost::optional<TextureFactory> mDepthTextureFactory;
	int mMultisampleSampleCount;
};

} // namespace vis
} // namespace skybolt
