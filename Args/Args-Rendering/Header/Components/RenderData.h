#pragma once
#include <Args-Core.h>

namespace Args
{
	class Mesh;
	class Material;
	struct Renderable;

	struct RenderData : public GlobalComponent<RenderData>
	{
		std::unordered_map<std::string, std::unordered_map<std::string, std::set<Entity*>>> batches;

		// Inherited via GlobalComponent
		virtual std::string ObjectType() override;
		virtual bool SetData(const std::string& name, const std::string& value) override;
		virtual bool GetData(const std::string& name, std::string& value) override;
	};
}