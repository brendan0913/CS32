// ChatTracker.cpp

#include "ChatTracker.h"
#include <string>
#include <vector>
#include <list>
#include <functional>
using namespace std;

// This class does the real work of the implementation.

class ChatTrackerImpl
{
public:
    ChatTrackerImpl(int maxBuckets);
    void join(string user, string chat);
    int terminate(string chat);
    int contribute(string user);
    int leave(string user, string chat);
    int leave(string user);
private:
    struct Info
    {
        Info(string u, string c) : user(u), chat(c), count(0), isActive(true) {}
        string user;
        string chat;
        int count;
        bool isActive; // used to see if the user left the chat (if the user is active, then the user
        // has not left the chat)
    };
    unsigned int mapFunction(const std::string& h);
    vector<list<Info>> usersToChats; // one hash table implementation
    int numOfBuckets;
};

ChatTrackerImpl::ChatTrackerImpl(int maxBuckets) : numOfBuckets(maxBuckets)
{ 
    usersToChats.resize(maxBuckets); // vector of buckets that match users to chats
}

unsigned int ChatTrackerImpl::mapFunction(const string& h)
{
    hash<string> str_hash; // string hasher
    unsigned int hashValue = str_hash(h);
    unsigned int bucketNum = hashValue % numOfBuckets; // number of buckets
    return bucketNum;
}

void ChatTrackerImpl::join(string user, string chat)
{
    // apply hash function to find index for given user
    int listNum = mapFunction(user);
    list<Info>::iterator it = usersToChats[listNum].begin();
    while (it != usersToChats[listNum].end())
    { // if the user and chat combination are already present in the list (previously joined and have not left)
        if (user == it->user && chat == it->chat && it->isActive)
        { // the new chat with active true is pushed to the front,; it is now the current chat, 
            // so it is pushed to the front regardless of where the original info node was in the list
            Info i = Info(user, chat);
            i.count = it->count; // the new info with the original count is pushed to the front
            // the other info is erased after the count is set
            usersToChats[listNum].erase(it);
            usersToChats[listNum].push_front(i);
            return;
        }
        it++;
    }
    //if the user and chat are completely new then the user is made active in the chat and it is pushed
    // to the front, to be made as the current chat
    Info i = Info(user, chat);
    usersToChats[listNum].push_front(i);
}

int ChatTrackerImpl::terminate(string chat)
{
    int total = 0;
    for (int i = 0; i < numOfBuckets; i++) // max num of bucktets
    {
        list<Info>::iterator it = usersToChats[i].begin();
        // goes through each bucket and each info item in the lists to find the given chat to terminate
        if (usersToChats[i].empty()) continue;
        while (it != usersToChats[i].end())
        {
            if (chat == it->chat)
            {
                total += it->count; // the number of contributions of the chat will be returned
                it = usersToChats[i].erase(it);
            }
            else
                it++;
        }
    }
    return total;
}

int ChatTrackerImpl::contribute(string user)
{
    // apply hash function to find index for given user
    int listNum = mapFunction(user);
    list<Info>::iterator it = usersToChats[listNum].begin();
    while (it != usersToChats[listNum].end())
    {
        if (user == it->user && it->isActive) // current chat, first instance of the user and active is true
        {
           it->count++; // increment the count of contributions and return the count
           return it->count;
        }
        it++;
    }
    return 0; // if there is no active chat for the user, there is no current chat
}

int ChatTrackerImpl::leave(string user, string chat)
{
    // apply hash function for given user
    int listNum = mapFunction(user);
    list<Info>::iterator it = usersToChats[listNum].begin();
    while (it != usersToChats[listNum].end())
    {
        // if node found, and the user is active in the chat (hasnt left)
        // the specified chat is left
        if (user == it->user && chat==it->chat && it->isActive)
        {
            it->isActive = false; // the user is now inactive in the chat, but the node is not erased
            return it->count; // returns number of contributions
        }
        it++;
    }
    return -1; // if there is no matching info item for the chat in which the user is active in, return -1
}

int ChatTrackerImpl::leave(string user)
{
    // apply hash function for given user
    int listNum = mapFunction(user);
    list<Info>::iterator it = usersToChats[listNum].begin();
    while (it != usersToChats[listNum].end())
    {
        //if info node found and the user is active (hasnt left)
        if (user == it->user && it->isActive) // leave current chat, the first one in the list is the 
            // most recent chat joined, so is it the current chat
        {
            it->isActive = false; // the user is now inactive in the chat, but the node is not erased
            return it->count; // number of contributions
        }
        it++;
    }
    return -1; // if there is no current chat (no instance of the user being active in a chat), return -1
}

//*********** ChatTracker functions **************

// These functions simply delegate to ChatTrackerImpl's functions.
// You probably don't want to change any of this code.

ChatTracker::ChatTracker(int maxBuckets)
{
    m_impl = new ChatTrackerImpl(maxBuckets);
}

ChatTracker::~ChatTracker()
{
    delete m_impl;
}

void ChatTracker::join(string user, string chat)
{
    m_impl->join(user, chat);
}

int ChatTracker::terminate(string chat)
{
    return m_impl->terminate(chat);
}

int ChatTracker::contribute(string user)
{
    return m_impl->contribute(user);
}

int ChatTracker::leave(string user, string chat)
{
    return m_impl->leave(user, chat);
}

int ChatTracker::leave(string user)
{
    return m_impl->leave(user);
}
