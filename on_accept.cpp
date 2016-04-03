//
//  on_accept.cpp
//
//
//  Created by sSTIs on 05.03.16.
//
//

#include "server_socket.h"

using namespace std;

//request analisys, returns true, if it has file to send
int MyServerSocket_select::on_accept(IOSocket_select *pSocket)
{
    int request_type = 0;// 1 get, 2 head, 0 other
    int content_type = 0;
    struct stat file_info;
    int file_descriptor;
    char file_size[20];
    char file_name[256];
    
    char *part_buffer;
    char sep[] = " \r\n";
    
    cerr << "Request" << endl;
    cerr << pSocket -> get_buffer();
    
    part_buffer = strtok(pSocket -> get_buffer(), sep);
    if (!strcmp(part_buffer, "GET"))
        request_type = 1;
    if (!strcmp(part_buffer, "HEAD"))
        request_type = 2;
    
    if (request_type == 0)
    {
        Response answer(501);
        pSocket -> send_response(answer.get_buffer());
        return 0;
    }
    
    part_buffer = strtok(NULL, sep);

    if (!strcmp(part_buffer, "/"))
        file_descriptor = ::open("./index.html", O_RDONLY);
    else
    {
        strcpy(file_name, ".");
        strcat(file_name, part_buffer);
        file_descriptor = ::open(file_name, O_RDONLY);
    }
    
    if (file_descriptor >= 0)
    {
        content_type = get_content_type(part_buffer);
        if (content_type == 0)
        {
            ::close(file_descriptor);
            file_descriptor = ::open("./forbidden.html", O_RDONLY);
        }
        fstat(file_descriptor, &file_info);
        sprintf(file_size, "%lld", file_info.st_size);
        pSocket -> body_size = file_info.st_size;
        
        if (content_type != 0)
        {
            Response answer(200, file_size,  content_type, request_type == 1?true:false, &(file_info.st_mtime));
            pSocket -> send_response(answer.get_buffer());
        }
        else
        {
            Response answer(403, file_size, 2);
            pSocket -> send_response(answer.get_buffer());
        }
    }
    else
    {
        file_descriptor = ::open("./not_found.html", O_RDONLY);
        fstat(file_descriptor, &file_info);
        sprintf(file_size, "%lld", file_info.st_size);
        pSocket -> body_size = file_info.st_size;

        Response answer(404, file_size, 2);
        pSocket -> send_response(answer.get_buffer());
    }

    if (request_type == 1)
    {
        pSocket -> file_descriptor = file_descriptor;
        return file_descriptor;
    }
    else
    {
        ::close(file_descriptor);
        return 0;
    }
}

int MyServerSocket_select::get_content_type(char *filename)
{
    char *file_type;
    if (!strcmp(filename, "/"))
        return 2;
    file_type = strrchr(filename, '.');
    if (file_type)
    {
        if (!strcmp(file_type, ".txt"))
            return 1;
        else if (!strcmp(file_type, ".html"))
            return 2;
        else if (!strcmp(file_type, ".jpg") || !strcmp(file_type, ".ico"))
            return 3;
        else
            return 0;
    }
    else
        return 0;
}

//class Response
//!--------------------------------------------------------------------------------------
Response::Response(int resp_type, char *cont_length, int cont_type, bool is_get1, time_t *last_modif)
{
    buffer = new char[10240];
    response_type = resp_type;
    content_length = cont_length;
    content_type = cont_type;
    last_modified = last_modif;
    is_get = is_get1;
    
    strcpy(buffer, "HTTP/1.1 ");
    switch (resp_type)
    {
        case 200:
            strcat(buffer, "200 OK\r\n");
            break;
        case 400:
            strcat(buffer, "400 Bad request\r\n");
            break;
        case 403:
            strcat(buffer, "403 Forbidden\r\n");
            break;
        case 404:
            strcat(buffer, "404 Not Found\r\n");
            break;
        case 501:
            strcat(buffer, "501 Not Implemented\r\n");
            break;
        default:
            break;
    }
    
    switch (resp_type) {
        case 200:
            add_Date();
            add_Server();
            if (is_get)
                add_Content_length();
            add_Last_modified();
            add_end();
            break;
        case 400:
        case 403:
        case 404:
            add_Date();
            add_Server();
            add_Content_type();
            add_Content_length();
            add_end();
            break;
        case 501:
            add_Date();
            add_Server();
            add_Allow();
            add_Content_type();
            add_Content_length();
            add_end();
            break;
        default:
            break;
    }
}

char *Response::get_buffer()
{
    return buffer;
}

void Response::add_Date()
{
    time_t rawtime;
    struct tm * timeinfo;
    char time_buffer [32];
    
    time (&rawtime);
    timeinfo = localtime(&rawtime);
    
    strftime (time_buffer, 32, "Date: %a, %d %b %G %T GTM", timeinfo);
    strcat(buffer, time_buffer);
    strcat(buffer, "\r\n");
}

void Response::add_Server()
{
    strcat(buffer, "Server: Model HTTP Server/2.0\r\n");
}

void Response::add_Content_type()
{
    switch (content_type) {
        case 1:
            strcat(buffer, "Content-Type: text/plain\r\n");
            break;
        case 2:
            strcat(buffer, "Content-Type: text/html\r\n");
            break;
        case 3:
            strcat(buffer, "Content-Type: image/jpeg\r\n");
            break;
        default:
            break;
    }
}

void Response::add_Content_length()
{
    strcat(buffer, "Content-Length: ");
    if (content_length == NULL)
        strcat(buffer, "0");
    else
        strcat(buffer, content_length);
    strcat(buffer, "\r\n");
}

void Response::add_Allow()
{
    strcat(buffer, "Allow: GET, HEAD\r\n");
}

void Response::add_Last_modified()
{
    strcat(buffer, "Last-modified: ");
    struct tm * timeinfo;
    char time_buffer [32];
    
    timeinfo = localtime(last_modified);
    strftime (time_buffer, 32, "%a, %d %b %G %T GTM\r\n", timeinfo);
    strcat(buffer, time_buffer);
}

void Response::add_end()
{
    strcat(buffer, "\r\n\0");
}

Response::~Response()
{
    delete [] buffer;
}
