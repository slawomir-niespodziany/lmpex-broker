#include <chrono>
#include <iostream>
#include <thread>
#include "abstract_link.h"
#include "lmp/lmp.h"

using namespace std::string_literals;
using namespace std::chrono_literals;

struct Message {
    const std::string payload;   // Data to be sent.
    const bool priority;         // True when the message is high priority.
    bool sent;                   // True when message was succesfully sent.
};

int main(const int argc, const char *argv[]) {
    lmp::ModuleRegistry<AbstractLink> &linkRegistry = lmp::ModuleRegistry<AbstractLink>::getInstance();
    std::unordered_set<AbstractLink::id_type> linkIds = linkRegistry.all();

    {   // Print all registered link id`s.
        std::cout << "Available links: "s;
        for (const auto &linkId : linkIds) {
            std::cout << linkId << ' ';
        }
        std::cout << std::endl;
    }

    std::shared_ptr<AbstractLink> pMainLink, pBackupLink;

    {   // Select main link.
        std::string mainLinkId = ""s;

        {   // Check if default id for main link is registered.
            const std::string mainLinkIdDefault = "eth"s;

            if (0 != linkIds.count(mainLinkIdDefault)) {
                mainLinkId = mainLinkIdDefault;
            }
        }

        // Check if user id for main link is provided and registered.
        if (2 <= argc) {
            const std::string mainLinkIdUser = argv[1];

            if (0 != linkIds.count(mainLinkIdUser)) {
                mainLinkId = mainLinkIdUser;
            }
        }

        // Assign main link.
        if (""s != mainLinkId) {
            pMainLink = linkRegistry.get(mainLinkId);
        }
    }

    {   // Select backup link.
        std::string backupLinkId = ""s;

        {   // Check if default id for main link is registered.
            const std::string backupLinkIdDefault = ""s;

            if (0 != linkIds.count(backupLinkIdDefault)) {
                backupLinkId = backupLinkIdDefault;
            }
        }

        // Check if user id for main link is provided and registered.
        if (3 <= argc) {
            const std::string backupLinkIdUser = argv[2];

            if (0 != linkIds.count(backupLinkIdUser)) {
                backupLinkId = backupLinkIdUser;
            }
        }

        // Assign main link.
        if (""s != backupLinkId) {
            pBackupLink = linkRegistry.get(backupLinkId);
        }
    }

    std::cout << "Main link: "s << ((nullptr != pMainLink) ? (pMainLink->getId()) : ("-"s)) << ", "s   //
              << "backup link: "s << ((nullptr != pBackupLink) ? (pBackupLink->getId()) : ("-"s))      //
              << std::endl;

    Message messages[] = {{"Msg  1"s, false, false},   //
                          {"Msg *2"s, true, false}};

    if (nullptr != pMainLink) {
        bool messagesPending = true;

        while (messagesPending) {
            messagesPending = false;

            for (Message &message : messages) {
                if (!message.sent) {
                    if (pMainLink->sendMessage(message.payload)) {
                        message.sent = true;

                    } else if ((nullptr != pBackupLink) && (message.priority)) {
                        if (pBackupLink->sendMessage(message.payload)) {
                            message.sent = true;
                        }
                    }
                }

                if (!message.sent) {
                    messagesPending = true;
                }
            }

            std::this_thread::sleep_for(1s);
        }
    }

    return 0;
}
