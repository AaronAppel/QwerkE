#pragma once

#include <string>

namespace QwerkE {

    namespace Network {

        void Initialize();
        void TearDown();

        void TestUpdate();

        void ConnectToServer();
        void DisconnectFromServer();

        void StartServer();
        void StopServer();

        bool IsInitialized();
        bool IsServer();

    }

}
