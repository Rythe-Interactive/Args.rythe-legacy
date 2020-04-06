#include "Input/InputSystem.h"
#include "Components/Input.h"
#include <GLFW/glfw3.h>

using namespace rapidjson;

Args::InputSystem::InputSystem() : MonoUpdateSystem<InputSystem>()
{
	Engine::BindToEvent<Events::ControllerConnected>(std::bind(&InputSystem::OnConnect, this, std::placeholders::_1));
};

void Args::InputSystem::Init()
{
	BindForUpdate(std::bind(&InputSystem::Update, this, std::placeholders::_1));

	////Engine::BindToEvent<Events::ControllerDisconnected>(std::bind(&InputSystem::OnControllerDisconnected, this, std::placeholders::_1));
	//jsonLoader = JSONLoader();
	//Document dom;
	//std::string str = jsonLoader.LoadKeyMap("KeyMapTest.json");
	////Debug::Log(DebugInfo, str);
	//const char* toParse = str.c_str();
	//dom.Parse(toParse);
	//assert(dom.HasMember("KeyMaps"));
	//const Value& keyMaps = dom["KeyMaps"];
	////Debug::Log(DebugInfo, "Checking if Array");
	//assert(keyMaps.IsArray());
	////Debug::Log(DebugInfo, "Is Array");
	//SizeType i = 0;
	//for (i = 0; i < keyMaps.Size(); i++)
	//{
	//	//Debug::Log(DebugInfo, "Checking if keyMaps[i] is and Object");
	//	assert(keyMaps[i].IsObject());
	//	//Debug::Log(DebugInfo, "True");

	//	//Debug::Log(DebugInfo, "Checking if keyMaps[i][Pair] is an array");
	//	assert(keyMaps[i]["Pair"].IsArray());
	//	//Debug::Log(DebugInfo, "True");

	//	//Debug::Log(DebugInfo,"Setting pair to pair var");
	//	const Value& pair = keyMaps[i]["Pair"];
	//	//Debug::Log(DebugInfo, "Done");

	//	//Debug::Log(DebugInfo,"Checking if pair is array");
	//	assert(pair.IsArray());
	//	//Debug::Log(DebugInfo, "True");

	//	//Debug::Log(DebugInfo, "Checking if first in Pair is a string");
	//	assert(pair[0].IsString());
	//	//Debug::Log(DebugInfo, "True");

	//	//Debug::Log(DebugInfo, "Checking if second in Pair is a int");
	//	assert(pair[1].IsString());
	//	//Debug::Log(DebugInfo, "True");

	//	//convert_string(pair[1].GetString(),key);
	//	enumStorage[pair[0].GetString()] = W;
	//	//Debug::Log(DebugInfo, "i%", buttonMap[pair[0].GetString()]);
	//}

	Input* inputData = GetGlobalComponent<Input>();

	inputData->MapActionInput("Exit", 0);
	inputData->MapActionInput("Square", 2);
	inputData->MapActionInput("Triangle", 3);
	inputData->MapAxisInput("MoveX", 16, 1);
	inputData->MapAxisInput("MoveY", 17, 1);
	inputData->MapAxisInput("RotateX", 18, 1);
	inputData->MapAxisInput("RotateY", 19, 1);
	inputData->MapAxisInput("MoveUp", 21, 1);
	inputData->MapAxisInput("MoveDown", 20, 1);

	Debug::Success(DebugInfo, "Initialised InputSystem");

}

void Args::InputSystem::Update(float deltaTime)
{
	Input* inputData = GetGlobalComponent<Input>();

	for (auto iterator = inputData->controllers.begin(); iterator != inputData->controllers.end(); ++iterator)
	{
		ControllerID controllerId = *iterator;

		GLFWgamepadstate state;
		if (glfwGetGamepadState(controllerId, &state) == GLFW_TRUE)
		{
			// buttons
			for (InputCode input = 0; input < 16; input++)
			{
				if (!inputData->actionMapping.count(input))
					continue;

				bool firstFrame = false;

				if (state.buttons[input] == GLFW_PRESS && !inputData->registeredInputs[controllerId].count(input))
				{
					firstFrame = true;

					for (auto callback : inputData->actionCallbacks[inputData->actionMapping[input]])
						callback(controllerId, ActionState::PRESS);

					//Debug::Log(DebugInfo, "Controller %i button %i : PRESS", (int)controllerId, input);

					inputData->registeredInputs[controllerId].insert(input);
				}
				else if (state.buttons[input] == GLFW_RELEASE && inputData->registeredInputs[controllerId].count(input))
				{
					for (auto callback : inputData->actionCallbacks[inputData->actionMapping[input]])
						callback(controllerId, ActionState::RELEASE);

					//Debug::Log(DebugInfo, "Controller %i button %i : RELEASE", (int)controllerId, input);

					inputData->registeredInputs[controllerId].erase(input);
				}

				if (!firstFrame && inputData->registeredInputs[controllerId].count(input))
				{
					//Debug::Log(DebugInfo, "Controller %i button %i : HOLD", (int)controllerId, input);

					for (auto callback : inputData->actionCallbacks[inputData->actionMapping[input]])
						callback(controllerId, ActionState::HOLD);
				}
			}

			//axes
			for (InputCode input = 16; input < 22; input++)
			{
				if (!inputData->axisMapping.count(input))
					continue;

				AxisValue value = state.axes[input - 16];

				if (abs(value) <= 0.05f)
					value = 0;

				if (inputData->registeredInputs[controllerId].count(input) || value != 0)
				{
					if (value == 0)
						inputData->registeredInputs[controllerId].erase(input);
					else if (!inputData->registeredInputs[controllerId].count(input))
						inputData->registeredInputs[controllerId].insert(input);

					inputData->axisValues[inputData->axisMapping[input].first][controllerId] = value;

					AxisValue scaledValue = inputData->axisMapping[input].second * value;

					for (auto callback : inputData->axisCallbacks[inputData->axisMapping[input].first])
						callback(controllerId, scaledValue);

					//Debug::Log(DebugInfo, "Controller %i axis %i :%f", (int)controllerId, input - 16, scaledValue);
				}
			}
		}
		else
		{
			if (inputData->controllers.count(controllerId))
				inputData->controllers.erase(controllerId);

			if (inputData->controllers.empty())
				break;
		}
	}
}

void Args::InputSystem::OnConnect(IEvent* event)
{
	Events::ControllerConnected* controllerEvent = dynamic_cast<Events::ControllerConnected*>(event);

	if (controllerEvent->eventType == GLFW_CONNECTED && glfwJoystickIsGamepad(controllerEvent->controllerID))
	{
		Debug::Log(DebugInfo, "Controller %i connected", (int)controllerEvent->controllerID);

		GetGlobalComponent<Input>()->controllers.insert((ControllerID)controllerEvent->controllerID);
	}
	else if (controllerEvent->eventType == GLFW_DISCONNECTED)
	{
		Debug::Log(DebugInfo, "Controller %i disconnected", (int)controllerEvent->controllerID);

		Input* inputData = GetGlobalComponent<Input>();
		if (inputData->controllers.count((ControllerID)controllerEvent->controllerID))
			inputData->controllers.erase((ControllerID)controllerEvent->controllerID);
	}
}

void Args::InputSystem::RetrieveInput()
{
	releasedKeys.clear();
	pressedKeys.clear();
	for (int controller = 0; controller < 16; controller++)
	{
		if (glfwJoystickIsGamepad(controller))
		{
			if (!isConnected)
			{
				Debug::Log(DebugInfo, "The Joystick is Connected");
			}
			isConnected = true;

			GLFWgamepadstate state;
			if (glfwGetGamepadState(controller, &state) == GLFW_TRUE)
			{
				for (int i = 0; i < 15; i++)
				{
					if (state.buttons[i] == GLFW_PRESS && !keyPressed[i].count(controller))
					{
						Debug::Log(DebugInfo, "%i %u PressStarted", controller, i);
						keyPressed[i].insert(controller);
						pressedKeys[glfwToKey[i]].first = glfwToKey[i];
						pressedKeys[glfwToKey[i]].second.push_back(controller);
						continue;
					}
					if (state.buttons[i] == GLFW_RELEASE && keyPressed[i].count(controller))
					{
						Debug::Log(DebugInfo, "%i %u PressEnded", controller, i);
						keyPressed[i].erase(controller);
						releasedKeys[glfwToKey[i]].first = glfwToKey[i];
						releasedKeys[glfwToKey[i]].second.push_back(controller);
						continue;
					}
				}

				axisMap[JOYSTICK_LEFT_X] = state.axes[0];
				axisMap[JOYSTICK_LEFT_Y] = state.axes[1];
				axisMap[JOYSTICK_RIGHT_X] = state.axes[2];
				axisMap[JOYSTICK_RIGHT_Y] = state.axes[3];
				axisMap[BUMPER_L2] = state.axes[4];
				axisMap[BUMPER_R2] = state.axes[5];
			}
		}
	}
}

void Args::InputSystem::BindFunction(std::string name, std::function<void()> func, bool onPress)
{
	BindFunctionToAction(enumStorage[name], func, onPress);
}
void Args::InputSystem::BindFunction(std::string name, std::function<void(float)> func)
{
	BindFunctionToAxis(enumStorage[name], func);
}
void Args::InputSystem::BindFunctionToAction(Args::Key key, std::function<void()> func, bool onPress)
{
	if (onPress)
	{
		if (pressedKeys[key].first == key)
		{
			actionMap[key] = func;
		}
	}
	else
	{
		if (releasedKeys[key].first == key)
		{
			actionMap[key] = func;
		}
	}
}
void Args::InputSystem::BindFunctionToAxis(Key key, std::function<void(float)> func)
{
	axisActionMap[key] = func;
}
void Args::InputSystem::InvokeAction(Key key, bool onPress)
{
	if (actionMap[key] != NULL)
	{
		if (onPress)
		{
			if (pressedKeys[key].first == key)
			{
				actionMap[key]();
			}
		}
		else
		{
			if (releasedKeys[key].first == key)
			{
				actionMap[key]();
			}
		}
	}
}
void Args::InputSystem::InvokeAxis(Key key)
{
	if (axisActionMap[key] != NULL)
	{
		if (key >= 1000 && key < 1004)
		{
			//Debug::Log(DebugInfo, "Checking Analogs");
			if (abs(axisMap[key]) > 0.00015f)
			{
				axisActionMap[key](axisMap[key]);
				//Debug::Log(DebugInfo, "Analog");
				return;
			}
		}
		else if (key >= 1004)
		{
			//Debug::Log(DebugInfo, "Checking Triggers");
			if (abs(axisMap[key]) < 1.0000f)
			{
				axisActionMap[key](axisMap[key]);
				//Debug::Log(DebugInfo, "Trigger");
				return;
			}
		}
	}
	//Debug::Log(DebugInfo, "DoSomething \n");
}
void Args::InputSystem::doSomething()
{
	Debug::Log(DebugInfo, "DidSomething");
}
void Args::InputSystem::axisDoSomething(float axis)
{
	Debug::Log(DebugInfo, "Axis: %f", axis);
}







