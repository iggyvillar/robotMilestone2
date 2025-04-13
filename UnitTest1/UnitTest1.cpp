#include "pch.h"
#include "CppUnitTest.h"
#include "../robotMilestone2/mySocket.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace MySocketTests
{
    TEST_CLASS(MySocketTests)
    {
    public:

        // test socket creation
        TEST_METHOD(TestCreateSocket)
        {
            MySocket socket(SocketType::CLIENT, "127.0.0.1", 5000, ConnectionType::UDP);
            Assert::AreEqual(std::string("127.0.0.1"), socket.GetIPAddr());
            Assert::AreEqual(5000, socket.GetPort());
            Assert::AreEqual(static_cast<int>(SocketType::CLIENT), static_cast<int>(socket.GetType()));
        }

        // test change port before connecting
        TEST_METHOD(TestChangePort)
        {
            MySocket socket(SocketType::CLIENT, "127.0.0.1", 5000, ConnectionType::UDP);
            socket.SetPort(6000);
            Assert::AreEqual(6000, socket.GetPort());
        }

        // test changes the socket type before connecting
        TEST_METHOD(TestChangeType)
        {
            MySocket socket(SocketType::CLIENT, "127.0.0.1", 5000, ConnectionType::UDP);
            socket.SetType(SocketType::SERVER);
            Assert::AreEqual(static_cast<int>(SocketType::SERVER), static_cast<int>(socket.GetType()));
        }

        // send tcp data without connection established
        TEST_METHOD(TestSendWithoutTCPConnect)
        {
            MySocket socket(SocketType::CLIENT, "127.0.0.1", 5000, ConnectionType::TCP);
            const char* testData = "test";
            socket.SendData(testData, 4);
            Assert::IsTrue(true); // Should not crash
        }

        //test tcp
        TEST_METHOD(TestTCP)
        {
            // start TCP server on port 8080
            MySocket server(SocketType::SERVER, "127.0.0.1", 8080, ConnectionType::TCP);

            // start TCP client to same port
            MySocket client(SocketType::CLIENT, "127.0.0.1", 8080, ConnectionType::TCP);

            client.ConnectTCP();  // client attempts to connect
            server.ConnectTCP();  // Server accepts

            const char* msg = "hello";
            client.SendData(msg, 5);  // send "hello"

            char buffer[DEFAULT_SIZE] = {};
            int bytes = server.GetData(buffer);  // server receives

            Assert::AreEqual(5, bytes);
            Assert::AreEqual(std::string("hello"), std::string(buffer, 5));
        }

        // change ip before connecting
        TEST_METHOD(TestChangeIP)
        {
            MySocket socket(SocketType::CLIENT, "127.0.0.1", 5000, ConnectionType::UDP);
            socket.SetIPAddr("192.168.0.100");
            Assert::AreEqual(std::string("192.168.0.100"), socket.GetIPAddr());
        }
       
        // TCP send/receive without connection handles without crashing the whole thing
        TEST_METHOD(TestTCPNoCrash)
        {
            MySocket socket(SocketType::CLIENT, "127.0.0.1", 5000, ConnectionType::TCP);
            const char* msg = "test";
            socket.SendData(msg, 4);
            char buffer[DEFAULT_SIZE] = {};
            int received = socket.GetData(buffer);
            Assert::IsTrue(received <= 0);
        }

        // UDP client sends, server receives and confirms message
        TEST_METHOD(TestUDPMessageReceive)
        {
            MySocket server(SocketType::SERVER, "127.0.0.1", 7777, ConnectionType::UDP);
            MySocket client(SocketType::CLIENT, "127.0.0.1", 7777, ConnectionType::UDP);

            const char* msg = "pong";
            client.SendData(msg, 4);

            char buffer[DEFAULT_SIZE] = {};
            int bytesReceived = server.GetData(buffer);

            Assert::AreEqual(4, bytesReceived);
            Assert::AreEqual(std::string("pong"), std::string(buffer, 4));
        }
    };
}

