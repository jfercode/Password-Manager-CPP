#ifndef AUTHMANAGER_HPP
# define AUTHMANAGER_HPP

#include "library.hpp"

class AuthenticationManager
{
    private:      
        
    public:
        AuthenticationManager();
        ~AuthenticationManager();

        //  Authenticate existed user
        bool    authenticateUser(const std::string &username, const std::string &password) const;
        
        //  Register a new user into the system 
        bool    registerNewUser(const std::string &username, const std::string &password, bool isMaster) const;
};

#endif // AUTHMANAGER_HPP