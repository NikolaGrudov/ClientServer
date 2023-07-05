class Server{
    public:
    
        Server() {}
        void listening(int socket);
        virtual void handle_connection(int sockfd);
        void create_server(); 
        ~Server() {}
};