#pragma once
#include <vector>
#include <memory>
#include <tuple>
#include <Utils/Common.h>

namespace Args
{
	template<typename ...Arguments>
	class DelegateBase
	{
	public:
		virtual void Invoke(Arguments... arguments) = 0;
	};

	template<class OwnerType, typename ReturnType, typename ...Arguments>
	class Delegate : public DelegateBase<Arguments...>
	{
		std::vector<FuncPtr<OwnerType, ReturnType, Arguments...>> funcPointers;

		std::vector<OwnerType*> owners;

		float ArgsAmount;
	public:
		Delegate();

		void Bind(OwnerType* object, ReturnType(OwnerType::* function)(Arguments...));

		virtual ReturnType* Invoke(Arguments... arguments) override;

		virtual ReturnType* operator()(Arguments... arguments);
	};

	template<class OwnerType, typename ReturnType, typename ...Arguments>
	Delegate<OwnerType, ReturnType, Arguments...>::Delegate()
	{

	}

	template<class OwnerType, typename ReturnType, typename ...Arguments>
	ReturnType* Delegate<OwnerType, ReturnType, Arguments...>::Invoke(Arguments... arguments)
	{
		ReturnType returnValues[owners.size];
		for (int i = 0; i < owners.size; i++)
		{
			OwnerType* object = owners[i];
			returnValues[i] = ((object)->*funcPointers[i])(arguments...);
		}
		return returnValues;
	}

	template<class OwnerType, typename ReturnType, typename ...Arguments>
	 ReturnType* Delegate<OwnerType, ReturnType, Arguments...>::operator()(Arguments... arguments)
	{
		return Invoke(arguments);
	}

	template<class OwnerType, typename ReturnType, typename ...Arguments>
	void Delegate<OwnerType, ReturnType, Arguments...>::Bind(OwnerType* object, ReturnType(OwnerType::* function)(Arguments...))
	{
		owners.push_back(object);
		funcPointers.push_back(function);
	}

}