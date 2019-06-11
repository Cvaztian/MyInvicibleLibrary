#include "restclient.h"
#include <string>
#include <iostream>

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
        return client.request(methods::GET, U(""), mensaje);
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

void RestClient::Put(nlohmann::json mensaje){
    // Json normal to Casablanca

    // Lo siguiente es necesario para mantener la imagen intacta
    std::vector<char> asd = mensaje["imagen"];
    std::vector<json::value> ll;
    for(int i = 0;i<asd.size();i++){
        ll.push_back(json::value(asd[i]));
    }
    // Listo

    json::value postData;

    postData["nombre"] = json::value::string(U(mensaje["nombre"]));
    postData["galeria"] = json::value::string(U(mensaje["galeria"]));
    postData["autor"] = json::value::string(U(mensaje["autor"]));
    postData["descripcion"] = json::value::string(U(mensaje["descripcion"]));
    postData["year"] = json::value::string(U(mensaje["year"]));
    postData["id"] = json::value((int)mensaje["id"]);
    postData["size"] = json::value((int)(mensaje["size"]));
    postData["imagen"] = json::value::array(ll);
    postData["mensaje"] = json::value::string(U(mensaje["mensaje"]));
    postData["protocolo"] = json::value((int)(mensaje["protocolo"]));



    auto fileStream = std::make_shared<ostream>();
    std::cout<< mensaje<<std::endl;
    // Open stream to output file.
    pplx::task<void> requestTask = fstream::open_ostream(U("results.html")).then([=](ostream outFile)
    {
        *fileStream = outFile;

        // Create http_client to send the request.
        http_client client(U("http://127.0.0.1:34568"));

        // Build request URI and start the request.
        uri_builder builder(U("/search"));
        builder.append_query(U("q"), U("cpprestsdk github"));
        return client.request(methods::PUT, U(""), postData.to_string().c_str(), U("application/json"));
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
        return client.request(methods::POST, U(""), mensaje);
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
        return client.request(methods::DEL, U(""), mensaje);
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
