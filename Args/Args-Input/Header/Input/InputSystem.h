#pragma once
#include <Args-Core.h>
#include <ECS/System.h>
#include <string>
#include <functional>
#include <list>
#include <array>
#include <map>
#include <unordered_map>
#include <vector>
#include "../../Args-Engine/Header/Serialisation/JSONLoader.h"
#include <rapidjson/document.h>
#include <WindowSystem.h>
#include <WindowEvents.h>

namespace Args
{
    #pragma region Key
    enum Key
    {
        F1 = 258,
        F2 = 259,
        F3 = 260,
        F4 = 261,
        F5 = 262,
        F6 = 263,
        F7 = 264,
        F8 = 265,
        F9 = 266,
        F10 = 267,
        F11 = 268,
        F12 = 269,
        LEFT = 285,
        UP = 283,
        RIGHT = 286,
        DOWN = 284,
        PAGE_UP = 104,
        PAGE_DOWN = 105,
        HOME = 106,
        END = 107,
        INSERT = 108,
        A = 65,
        B = 66,
        C = 67,
        D = 68,
        E = 69,
        F = 70,
        G = 71,
        H = 72,
        I = 73,
        J = 74,
        K = 75,
        L = 76,
        M = 77,
        N = 78,
        O = 79,
        P = 80,
        Q = 81,
        R = 82,
        S = 83,
        T = 84,
        U = 85,
        V = 86,
        W = 87,
        X = 88,
        Y = 89,
        Z = 90,
        ZERO = 48,
        ONE = 49,
        TWO = 50,
        THREE = 51,
        FOUR = 52,
        FIVE = 53,
        SIX = 54,
        SEVEN = 55,
        EIGHT = 56,
        NINE = 57,
        ESCAPE = 257,
        SPACE = 32,
        ENTER = 294,
        DELETEk = 127,
        BACKSPACE = 295,
        LEFT_CTRL = 289,
        RIGHT_CTRL = 290,
        NUMPAD_0 = 302,
        NUMPAD_1 = 303,
        NUMPAD_2 = 304,
        NUMPAD_3 = 305,
        NUMPAD_4 = 306,
        NUMPAD_5 = 307,
        NUMPAD_6 = 308,
        NUMPAD_7 = 309,
        NUMPAD_8 = 310,
        NUMPAD_9 = 311,
        MINUS = 314,
        PLUS = 315,

        LEFT_ALT = 291,
        RIGHT_ALT = 292,
        TAB = 293,
        LEFT_SHIFT = 287,
        RIGHT_SHIFT = 288,
        TILDE = 96,
        SQ_BRACKET_1 = 91,
        SQ_BRACKET_2 = 93,
        SEMICOLON = 59,
        APOSTROPHE = 39,
        COMMA = 44,
        DOT = 46,
        QUESTION_MARK = 47,

        PRESSED = 1,
        RELEASED = 0,

        ANALOG0 = 1000,
        ANALOG1 = 1001,
        ANALOG2 = 1002,
        ANALOG3 = 1003,
        ANALOG4 = 1004,
        ANALOG5 = 1005,
        ANALOG6 = 1006,
        ANALOG7 = 1007,
        ANALOG8 = 1008,
        ANALOG9 = 1009,

        JOYSTICK_LEFT_X = 1000,
        JOYSTICK_LEFT_Y = 1001,
        JOYSTICK_RIGHT_X = 1002,
        JOYSTICK_RIGHT_Y = 1003,
        BUMPER_L1 = 1004,
        BUMPER_R1 = 1005,

        DIGITAL0 = 1010,
        DIGITAL1 = 1011,
        DIGITAL2 = 1012,
        DIGITAL3 = 1013,
        DIGITAL4 = 1014,
        DIGITAL5 = 1015,
        DIGITAL6 = 1016,
        DIGITAL7 = 1017,
        DIGITAL8 = 1018,
        DIGITAL9 = 1019,
        DIGITAL10 = 1020,
        DIGITAL11 = 1021,
        DIGITAL12 = 1022,
        DIGITAL13 = 1023,
        DIGITAL14 = 1024,
        DIGITAL15 = 1025,
        DIGITAL16 = 1026,
        DIGITAL17 = 1027,
        DIGITAL18 = 1028,
        DIGITAL19 = 1029,

        GAMEPAD_L_UP = 1010,
        GAMEPAD_L_RIGHT = 1011,
        GAMEPAD_L_DOWN = 1012,
        GAMEPAD_L_LEFT = 1013,

        GAMEPAD_R_UP = 1014,
        GAMEPAD_R_RIGHT = 1015,
        GAMEPAD_R_DOWN = 1016,
        GAMEPAD_R_LEFT = 1017,

        BUMPER_L2 = 1018,
        BUMPER_R2 = 1019,

        GAMEPAD_MENU = 1020
    };
#pragma endregion

    using namespace std::placeholders;

	class InputSystem : public MonoUpdateSystem<InputSystem>
	{
	public:
        InputSystem();

        JSONLoader jsonLoader;
        using ControllerID = uint8;

		virtual void Init() override;

		void Update(float deltaTime);

        void OnConnect(IEvent* event);


        void RetrieveInput();
        bool isConnected = false;
        bool wasPressed = false;
        std::map<int,std::set<int>> keyPressed;
        std::map<int, Key> glfwToKey = {std::pair<int,Key>(0,DIGITAL0),std::pair<int,Key>(1,DIGITAL1),std::pair<int,Key>(2,DIGITAL2), 
                                                            std::pair<int,Key>(3,DIGITAL3),std::pair<int,Key>(4,DIGITAL4),std::pair<int,Key>(5,DIGITAL5),
                                                            std::pair<int,Key>(6,DIGITAL6),std::pair<int,Key>(7,DIGITAL7),std::pair<int,Key>(8,DIGITAL8),
                                                            std::pair<int,Key>(9,DIGITAL9), std::pair<int,Key>(10,DIGITAL10), std::pair<int,Key>(11,DIGITAL11),
                                                            std::pair<int,Key>(12,DIGITAL12), std::pair<int,Key>(13,DIGITAL13), std::pair<int,Key>(14,JOYSTICK_LEFT_X),
                                                            std::pair<int,Key>(15,JOYSTICK_LEFT_Y) , std::pair<int,Key>(16,JOYSTICK_RIGHT_X) , std::pair<int,Key>(17,JOYSTICK_RIGHT_Y),
                                                            std::pair<int,Key>(18,BUMPER_L2) , std::pair<int,Key>(19,BUMPER_R2) };

        //Bind Function to a Button 
        void BindFunction(std::string name,std::function<void()> func,bool onPresse = true);
        //Bind Function to a Axis
        void BindFunction(std::string name, std::function<void(float)> func);

    private:

        //void OnControllerConnected(IEvent& event);
        //void WhileControllerConnected(IEvent& event);
        //void OnControllerDisconnected(IEvent& event);

        void UpdateAxesForKey(Key key, ControllerID controllerID);
        void CreateEvent(std::string name);
        void MapEventToKeyAction(std::string name,Key key);
        void MapEventToKeyAxis(std::string name, Key key, float value);
        void BindFunctionToAction(Args::Key key, std::function<void()> func,bool onPress = true);
        void BindFunctionToAxis(Args::Key key, std::function<void(float)> func);
        //void BindFunctionToButtonEvent(std::string name, std::function<void()> func); 
        void InvokeAction(Key key, bool onPress);
        void InvokeAxis(Key key);
        void doSomething();
        void axisDoSomething(float axis);

    private:
        std::unordered_map<std::string, Key> enumStorage;
        std::unordered_map<Key,std::string> buttonMap;
        std::unordered_map<Key, std::function<void()>> actionMap;
        std::unordered_map<Key, std::function<void(float)>> axisActionMap;
        std::unordered_map<Key, float> axisMap;
        std::unordered_map<std::string, Key> events;

        std::unordered_map<Key, std::pair<Key,std::vector<ControllerID>>> pressedKeys;
        std::unordered_map<Key, std::pair<Key, std::vector<ControllerID>>> releasedKeys;

        std::unordered_map <std::string, std::unordered_map<Key, float>> axisStorage;
        std::unordered_map<std::string, std::vector<Key>> actionStorage;
	};



}