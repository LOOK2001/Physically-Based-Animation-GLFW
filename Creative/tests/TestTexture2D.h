#pragma once

#include "../Object.h"
#include "../pba/core/PbaThing.h"

#include <memory>

namespace pba {

	class TestTexture2D : public pba::PbaThingyDingy
	{
	public:
		TestTexture2D(const std::string nam = "SBDAreaClothThing");
		~TestTexture2D() {};

		void OnUpdate(float deltaTime) override;
		virtual void Display() override;

		virtual void solve() override;

	};

	PbaThing TextureThing();
}
