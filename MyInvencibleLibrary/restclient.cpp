#include "restclient.h"
#include <string>

std::string RestClient::respuesta = "";
RestClient::RestClient()
{
}

void RestClient::Get(std::string mensaje){
    auto fileStream = std::make_shared<ostream>();

    // Open stream to output file.
    pplx::task<void> requestTask = fstream::open_ostream(U("results.html")).then([=](ostream outFile)
    {
        *fileStream = outFile;

        // Create http_client to send the request.
        http_client client(U("http://127.0.0.1:34568"));

        // Build request URI and start the request.
        uri_builder builder(U("/search"));
        builder.append_query(U("q"), U("cpprestsdk github"));
        return client.request(methods::PUT, "", mensaje);
    })

    // Handle response headers arriving.
    .then([=](http_response response)
    {
        printf("Received response status code:%u\n", response.status_code());
      //  std::cout<<response.body().<<std::endl;
        // Write response body into the file.
        respuesta = response.extract_string().get();
        return response.body().read_to_end(fileStream->streambuf());
    })

    // Close the file stream.
    .then([=](size_t)
    {
        return fileStream->close();
    });

    // Wait for all the outstanding I/O to complete and handle any exceptions
    try
    {
        requestTask.wait();
    }
    catch (const std::exception &e)
    {
        printf("Error exception:%s\n", e.what());
    }

    return;

}

void RestClient::Put(std::string mensaje){
    auto fileStream = std::make_shared<ostream>();

    // Open stream to output file.
    pplx::task<void> requestTask = fstream::open_ostream(U("results.html")).then([=](ostream outFile)
    {
        *fileStream = outFile;

        // Create http_client to send the request.
        http_client client(U("http://127.0.0.1:34568"));

        // Build request URI and start the request.
        uri_builder builder(U("/search"));
        builder.append_query(U("q"), U("cpprestsdk github"));
        return client.request(methods::PUT, "", mensaje);
    })

    // Handle response headers arriving.
    .then([=](http_response response)
    {
        printf("Received response status code:%u\n", response.status_code());
      //  std::cout<<response.body().<<std::endl;
        // Write response body into the file.
        respuesta = response.extract_string().get();
        return response.body().read_to_end(fileStream->streambuf());
    })

    // Close the file stream.
    .then([=](size_t)
    {
        return fileStream->close();
    });

    // Wait for all the outstanding I/O to complete and handle any exceptions
    try
    {
        requestTask.wait();
    }
    catch (const std::exception &e)
    {
        printf("Error exception:%s\n", e.what());
    }

    return;


}

void RestClient::Post(std::string mensaje){
    auto fileStream = std::make_shared<ostream>();

    // Open stream to output file.
    pplx::task<void> requestTask = fstream::open_ostream(U("results.html")).then([=](ostream outFile)
    {
        *fileStream = outFile;

        // Create http_client to send the request.
        http_client client(U("http://127.0.0.1:34568"));

        // Build request URI and start the request.
        uri_builder builder(U("/search"));
        builder.append_query(U("q"), U("cpprestsdk github"));
        return client.request(methods::PUT, "", mensaje);
    })

    // Handle response headers arriving.
    .then([=](http_response response)
    {
        printf("Received response status code:%u\n", response.status_code());
      //  std::cout<<response.body().<<std::endl;
        // Write response body into the file.
        respuesta = response.extract_string().get();
        return response.body().read_to_end(fileStream->streambuf());
    })

    // Close the file stream.
    .then([=](size_t)
    {
        return fileStream->close();
    });

    // Wait for all the outstanding I/O to complete and handle any exceptions
    try
    {
        requestTask.wait();
    }
    catch (const std::exception &e)
    {
        printf("Error exception:%s\n", e.what());
    }

    return;
}

void RestClient::Delete(std::string mensaje){
    auto fileStream = std::make_shared<ostream>();

    // Open stream to output file.
    pplx::task<void> requestTask = fstream::open_ostream(U("results.html")).then([=](ostream outFile)
    {
        *fileStream = outFile;

        // Create http_client to send the request.
        http_client client(U("http://127.0.0.1:34568"));

        // Build request URI and start the request.
        uri_builder builder(U("/search"));
        builder.append_query(U("q"), U("cpprestsdk github"));
        return client.request(methods::PUT, "", mensaje);
    })

    // Handle response headers arriving.
    .then([=](http_response response)
    {
        printf("Received response status code:%u\n", response.status_code());
      //  std::cout<<response.body().<<std::endl;
        // Write response body into the file.
        respuesta = response.extract_string().get();
        return response.body().read_to_end(fileStream->streambuf());
    })

    // Close the file stream.
    .then([=](size_t)
    {
        return fileStream->close();
    });

    // Wait for all the outstanding I/O to complete and handle any exceptions
    try
    {
        requestTask.wait();
    }
    catch (const std::exception &e)
    {
        printf("Error exception:%s\n", e.what());
    }

    return;
}