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
		const uint32 externalTypeId;

		Entity* owner;

		uint32 id;

		IComponent(Entity* entity, uint32 typeId, uint32 externalId);
		virtual void CleanUp() {}
	};

	template<class Self>
	struct Component : public IComponent
	{
	public:
		static const std::string componentName;
		static uint32 typeId;
		static uint32 externalTypeId;

		Component(Entity* entity) : IComponent(entity, typeId, externalTypeId) {}

	protected:
	private:
	};

	struct IGlobalComponent : public ISerialisable
	{
	public:
		const uint32 typeID;
		const uint32 externalTypeId;
		uint32 id;

		IGlobalComponent(uint32 typeId, uint32 externalId) : typeID(typeId), id(0), externalTypeId(externalId) {}
		virtual void CleanUp() {}
	};


	template<class Self>
	struct GlobalComponent : public IGlobalComponent
	{
	public:
		static const std::string componentName;
		static uint32 typeId;
		static uint32 externalTypeId;

		GlobalComponent() : IGlobalComponent(typeId, externalTypeId) {}

	protected:
	private:
	};

	template<class Self>
	const std::string GlobalComponent<Self>::componentName(GetTypeName<Self>());

	template<class Self>
	uint32 GlobalComponent<Self>::typeId = 0;

	template<class Self>
	uint32 GlobalComponent<Self>::externalTypeId = 0;

	template<class Self>
	const std::string Component<Self>::componentName(GetTypeName<Self>());

	template<class Self>
	uint32 Component<Self>::typeId = 0;

	template<class Self>
	uint32 Component<Self>::externalTypeId = 0;
}