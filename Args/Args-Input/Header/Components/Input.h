#pragma once
#include <Args-Core.h>

namespace Args
{
	typedef uint8 ControllerID;
	typedef uint8 EventID;
	typedef int InputCode;
	typedef float AxisValue;

	class InputSystem;

	enum ActionState
	{
		PRESS, HOLD, RELEASE
	};

	struct Input : public GlobalComponent<Input>
	{
	private:
		friend class InputSystem;

		std::unordered_map<EventID, std::vector<std::function<void(ControllerID, ActionState)>>> actionCallbacks;
		std::unordered_map<EventID, std::vector<std::function<void(ControllerID, AxisValue)>>> axisCallbacks;

		std::unordered_map<EventID, std::unordered_map<ControllerID, AxisValue>> axisValues;

		std::unordered_map<std::string, EventID> eventIds;
		std::unordered_map<InputCode, EventID> actionMapping;
		std::unordered_map<InputCode, std::pair<EventID, AxisValue>> axisMapping;

		std::set<ControllerID> controllers;

		std::unordered_map<ControllerID, std::set<InputCode>> registeredInputs;

	public:

		void RegisterEvent(const std::string& name)
		{
			EventID id = (EventID)eventIds.size();
			eventIds[name] = id;
		}

		void MapActionInput(const std::string& name, InputCode input)
		{
			if (!eventIds.count(name))
				RegisterEvent(name);

			actionMapping[input] = eventIds[name];
		}

		void MapAxisInput(const std::string& name, InputCode input, AxisValue scale)
		{
			if (!eventIds.count(name))
				RegisterEvent(name);

			axisMapping[input] = std::make_pair(eventIds[name], scale);
		}

		void BindAction(const std::string& name, std::function<void(ControllerID, ActionState)> callback)
		{
			actionCallbacks[eventIds[name]].push_back(callback);
		}

		void BindAxis(const std::string& name, std::function<void(ControllerID, AxisValue)> callback)
		{
			axisCallbacks[eventIds[name]].push_back(callback);
		}

		// Inherited via GlobalComponent
		virtual std::string ObjectType() override;
		virtual bool SetData(const std::string& name, const std::string& value) override;
		virtual bool GetData(const std::string& name, std::string& value) override;
	};
}