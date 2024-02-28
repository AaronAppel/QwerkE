#include "QF_Network.h"

// https://github.com/facebookarchive/RakNet/tree/master/Samples
#include <stdio.h>
#include <cstdio>
#include <iostream>
#include <string.h>

#include "Libraries/RakNet/Source/RakPeerInterface.h"
#include "Libraries/RakNet/Source/RakNetTypes.h"
#include "Libraries/RakNet/Source/MessageIdentifiers.h"
#include "Libraries/RakNet/Source/BitStream.h"

#include "QF_Log.h"
#include "QF_Settings.h"

#define MAX_CLIENTS 10
#define SERVER_PORT 60000

namespace QwerkE {

    namespace Network {

        static std::string s_IP = "127.0.0.1";
        static bool s_IsServer = false;
        static bool s_Initialized = false;
        static RakNet::RakPeerInterface* s_Peer = nullptr;
        static RakNet::Packet* s_Packet = nullptr;

        enum GameMessages
        {
            ID_GAME_MESSAGE_1 = ID_USER_PACKET_ENUM,
            // ID_GAME_MESSAGE_2...
        };

        void Initialize()
        {
            if (!Settings::GetEngineSettings().networkingEnabled)
            {
                LOG_WARN("{0} Unable to initialize Network as networking is disabled", __FUNCTION__);
                return;
            }

            char str[512];
            s_Peer = RakNet::RakPeerInterface::GetInstance();

            while (true)
            {
                printf("(C)lient or (S)erver?\n");
                std::cin >> str;

                if ((str[0] == 'c') || (str[0] == 'C'))
                {
                    RakNet::SocketDescriptor sd;
                    s_Peer->Startup(1, &sd, 1);
                    s_IsServer = false;
                    break;
                }
                else if ((str[0] == 's') || (str[0] == 'S'))
                {
                    RakNet::SocketDescriptor sd(SERVER_PORT, 0);
                    s_Peer->Startup(MAX_CLIENTS, &sd, 1);
                    s_IsServer = true;
                    break;
                }
            }

            if (s_IsServer)
            {
                StartServer();
            }
            else
            {
                ConnectToServer();
            }

            s_Initialized = true;
        }

        void ConnectToServer()
        {
            if (s_Initialized)
            {
                if (!Settings::GetEngineSettings().networkingEnabled)
                {
                    LOG_WARN("{0} Unable to initialize Network as networking is disabled", __FUNCTION__);
                    return;
                }

                char str[512];
                printf("Enter server IP or press enter to auto detect IP\n");
                std::cin >> str;

                // TODO: Error handling
                if (str[0] == 'a' || str[0] == 'A')
                {
                    // strcpy_s(str, s_IP.c_str());
                    // s_IP = s_Peer->GetLocalIP(0); // TODO: Get local IP address
                }

                printf("Starting the client.\n");
                s_Peer->Connect(s_IP.c_str(), SERVER_PORT, 0, 0); // TODO: Handle local and external IP addresses
            }
        }

        void DisconnectFromServer()
        {
            if (s_Initialized)
            {
                // TODO: Properly disconnect from host
                // s_Peer->GetMyGUID()
                // AddressOrGUID target = s_Peer->GetSystemAddressFromIndex(0);
                // s_Peer->CloseConnection(, true, 0, LOW_PRIORITY);
                s_Peer->Shutdown(0);
                RakNet::RakPeerInterface::DestroyInstance(s_Peer);
                s_Initialized = false;
                printf("You have disconnected.\n");
            }
        }

        void StartServer()
        {
            if (s_Initialized)
            {
                printf("Starting the server.\n");
                // We need to let the server accept incoming connections from the clients
                s_Peer->SetMaximumIncomingConnections(MAX_CLIENTS);
            }
        }

        void StopServer()
        {
            if (s_Initialized)
            {
                // TODO: Properly disconnect from host
                // s_Peer->GetMyGUID()
                // AddressOrGUID target = s_Peer->GetSystemAddressFromIndex(0);
                // s_Peer->CloseConnection(, true, 0, LOW_PRIORITY);
                s_Peer->Shutdown(0);
                RakNet::RakPeerInterface::DestroyInstance(s_Peer);
                s_Initialized = false;
                printf("Server has been stopped.\n");
            }
        }

        bool IsInitialized()
        {
            return s_Initialized;
        }

        bool IsServer()
        {
            return s_IsServer;
        }

        void TestUpdate()
        {
            if (!s_Initialized)
                return;

            for (s_Packet = s_Peer->Receive(); s_Packet; s_Peer->DeallocatePacket(s_Packet), s_Packet = s_Peer->Receive())
            {
                switch (s_Packet->data[0])
                {
                case ID_REMOTE_DISCONNECTION_NOTIFICATION:
                    printf("Another client has disconnected.\n");
                    break;

                case ID_REMOTE_CONNECTION_LOST:
                    printf("Another client has lost the connection.\n");
                    break;

                case ID_REMOTE_NEW_INCOMING_CONNECTION:
                    printf("Another client has connected.\n");
                    break;

                case ID_CONNECTION_REQUEST_ACCEPTED:
                {
                    printf("Your connection request has been accepted.\n");

                    // Use a BitStream to write a custom user message
                    // Bitstreams are easier to use than sending casted structures, and handle endian swapping automatically
                    RakNet::BitStream bsOut;
                    bsOut.Write((RakNet::MessageID)ID_GAME_MESSAGE_1);
                    bsOut.Write("Hello world");
                    s_Peer->Send(&bsOut, HIGH_PRIORITY, RELIABLE_ORDERED, 0, s_Packet->systemAddress, false);
                }
                break;

                case ID_NEW_INCOMING_CONNECTION:
                    printf("A connection is incoming.\n");
                    break;

                case ID_NO_FREE_INCOMING_CONNECTIONS:
                    printf("The server is full.\n");
                    break;

                case ID_DISCONNECTION_NOTIFICATION:
                    if (s_IsServer)
                    {
                        printf("A client has disconnected.\n");
                    }
                    else
                    {
                        printf("You have been disconnected.\n");
                    }
                    break;

                case ID_CONNECTION_LOST:
                    if (s_IsServer)
                    {
                        printf("A client lost the connection.\n");
                    }
                    else {
                        printf("Connection lost.\n");
                    }
                    break;

                case ID_GAME_MESSAGE_1:
                {
                    RakNet::RakString rs;
                    RakNet::BitStream bsIn(s_Packet->data, s_Packet->length, false);
                    bsIn.IgnoreBytes(sizeof(RakNet::MessageID));
                    bsIn.Read(rs);
                    printf("%s\n", rs.C_String());
                }
                break;

                default:
                    printf("Message with identifier %i has arrived.\n", s_Packet->data[0]);
                    break;
                }
            }
        }

    }

}
