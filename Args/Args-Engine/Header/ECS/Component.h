#pragma once
#include "Serialisation/Serialisable.h"
#include "Utils/Common.h"

namespace Args
{
	class Entity;
	class ComponentManager;

	struct IComponent : public ISerialisable
	{
	protected:
		static uint32 componentTypeCount;

		ComponentManager* manager;

	public:
		const uint32 ownerID;
		const uint32 typeID;

		Entity* owner;

		uint32 id;

		IComponent(Entity* entity, uint32 typeId);
		virtual void CleanUp(){}
	};

	template<class Self>
	struct Component : public IComponent
	{
	public:
		static const std::string componentName;
		static uint32 typeId;

		Component(Entity* entity) : IComponent(entity, typeId) {}

	protected:
	private:
	};

	struct IGlobalComponent : public ISerialisable
	{
	public:
		const uint32 typeID;

		uint32 id;

		IGlobalComponent(uint32 typeId) : typeID(typeId), id(0) {}
		virtual void CleanUp() {}
	};


	template<class Self>
	struct GlobalComponent : public IGlobalComponent
	{
	public:
		static const std::string componentName;
		static uint32 typeId;

		GlobalComponent() : IGlobalComponent(typeId) {}

	protected:
	private:
	};

	template<class Self>
	const std::string GlobalComponent<Self>::componentName(GetTypeName<Self>());

	template<class Self>
	uint32 GlobalComponent<Self>::typeId = 0;

	template<class Self>
	const std::string Component<Self>::componentName(GetTypeName<Self>());

	template<class Self>
	uint32 Component<Self>::typeId = componentTypeCount++;
}