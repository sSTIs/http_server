//
//  cgi_handler.cpp
//
//
//  Created by sSTIs on 03.04.16.
//
//

#include "server_socket.h"

using namespace std;

void CGIHandler::run_cgi(char *file_name, char *args)
{
    int pid1 = fork();
    if (pid1 < 0)
    {
        perror("Fork error");
        exit(10);
    }
    else if (pid1 == 0)
    {
        //make env
        char **env;
        env = new char* [16];
        for (int i = 0; i < 16; ++i)
            env[i] = new char [1024];

        env[15] = NULL;

        strcpy(env[0], "CONTENT_TYPE=plain/html");

        strcpy(env[1], "GATEWAY_INTERFACE=CGI/1.1");
        strcpy(env[2], "REMOTE_ADDR=127.0.0.1");
        strcpy(env[3], "REMOTE_PORT=8000");

        strcpy(env[4], "QUERY_STRING=");
        if (args != NULL)
            strcat(env[4], args);
        strcpy(env[5], "SERVER_NAME=localhost:8000");
        strcpy(env[6], "SERVER_ADDR=127.0.0.1");
        strcpy(env[7], "SERVER_PORT=8000");
        strcpy(env[8], "SERVER_PROTOCOL=HTTP/1.1");
        strcpy(env[9], "SERVER_SOWTWARE=who/knows");
        
        strcpy(env[10], "SCRIPT_NAME=");
        strcat(env[10], file_name);
        strcat(env[12], "DOCUMENT_ROOT=/Users/user/Documents/CMC/prac4/server/server");
        
        strcpy(env[11], "SCRIPT_FILENAME=");
        strcat(env[11], env[12]);
        strcat(env[11], env[10]);
        strcpy(env[13], "HTTP_USER_AGENT=Mozilla/5.0 (Macintosh; Intel Mac OS X 10_11_4) AppleWebKit/601.5.17 (KHTML, like Gecko) Version/9.1 Safari/601.5.17");
        strcpy(env[14], "HTTP_REFERER=http://localhost:8000/");

        //output redirect
        char redir_file_name[20];
        int redir_fd;
        strcpy(redir_file_name, "temp");
        char temp_str[20];
        sprintf(temp_str, "%d", getpid());
        strcat(redir_file_name, temp_str);
        redir_fd = open(redir_file_name, O_WRONLY | O_CREAT | O_TRUNC, 0644);
        if (redir_fd < 0)
        {
            perror("Open temporary file error");
            exit(11);
        }

        dup2(redir_fd, 1);
        //exec
        execve(file_name + 1, NULL, env);// +1 because first symbol '/'

        cerr << "Program wasn't execute";
        close(redir_fd);
        exit(12);
    }
    else
    {
        //parent
        pid = pid1;
    }
}

void CGIHandler::make_response(IOSocket_select *pSocket)
{
    struct stat file_info;
    char file_size[20];
    
    fstat(pSocket -> file_descriptor, &file_info);
    sprintf(file_size, "%lld", file_info.st_size - strlen("Content-type: text/html\r\n\r\n"));
    pSocket -> body_size = file_info.st_size;
    Response answer(200, file_size,  0, true, &(file_info.st_mtime));
    pSocket -> send_response(answer.get_buffer());
}
