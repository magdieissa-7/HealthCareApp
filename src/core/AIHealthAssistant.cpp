#include "AIHealthAssistant.h"
#include <boost/asio.hpp>
#include <boost/asio/ssl.hpp>
#include <boost/beast.hpp>
#include <boost/beast/ssl.hpp>
#include <iostream>
#include <thread>

namespace beast = boost::beast;
namespace http  = beast::http;
namespace net   = boost::asio;
namespace ssl   = boost::asio::ssl;
using tcp = net::ip::tcp;

AIHealthAssistant::AIHealthAssistant(const std::string &apiKey)
    : apiKey(apiKey) {}


std::string AIHealthAssistant::buildRequestBody(const std::string &question) const {

    std::string safe = question;
    std::string escaped;
    for (char c : safe) {
        if (c == '"')  escaped += "\\\"";
        else if (c == '\n') escaped += "\\n";
        else escaped += c;
    }

    return "{"
           "\"model\":\"claude-haiku-4-5-20251001\","
           "\"max_tokens\":512,"
           "\"system\":\"You are a helpful healthcare assistant. "
           "Provide clear, concise health information. "
           "Always remind users to consult a doctor for medical advice.\","
           "\"messages\":[{\"role\":\"user\",\"content\":\"" + escaped + "\"}]"
           "}";
}


std::string AIHealthAssistant::parseResponse(const std::string &json) const {

    const std::string key = "\"text\":\"";
    auto pos = json.find(key);
    if (pos == std::string::npos) {

        const std::string errKey = "\"message\":\"";
        auto errPos = json.find(errKey);
        if (errPos != std::string::npos) {
            errPos += errKey.size();
            auto end = json.find("\"", errPos);
            return "API Error: " + json.substr(errPos, end - errPos);
        }
        return "Could not parse response.";
    }
    pos += key.size();

    std::string result;
    while (pos < json.size()) {
        if (json[pos] == '\\' && pos + 1 < json.size()) {
            if (json[pos+1] == '"')  { result += '"';  pos += 2; }
            else if (json[pos+1] == 'n') { result += '\n'; pos += 2; }
            else if (json[pos+1] == '\\') { result += '\\'; pos += 2; }
            else { result += json[pos++]; }
        } else if (json[pos] == '"') {
            break;
        } else {
            result += json[pos++];
        }
    }
    return result;
}


void AIHealthAssistant::ask(
    const std::string &question,
    std::function<void(const std::string &, const std::string &)> callback)
{

    std::thread([this, question, callback]() {
        try {
            net::io_context ioc;
            ssl::context ctx(ssl::context::tlsv12_client);
            ctx.set_default_verify_paths();

            tcp::resolver resolver(ioc);
            beast::ssl_stream<beast::tcp_stream> stream(ioc, ctx);


            SSL_set_tlsext_host_name(stream.native_handle(), "api.anthropic.com");

            auto const results = resolver.resolve("api.anthropic.com", "443");
            beast::get_lowest_layer(stream).connect(results);
            stream.handshake(ssl::stream_base::client);

            std::string body = buildRequestBody(question);


            http::request<http::string_body> req{
                http::verb::post, "/v1/messages", 11};
            req.set(http::field::host,         "api.anthropic.com");
            req.set(http::field::content_type, "application/json");
            req.set("x-api-key",               apiKey);
            req.set("anthropic-version",       "2023-06-01");
            req.set(http::field::content_length,
                    std::to_string(body.size()));
            req.body() = body;
            req.prepare_payload();

            http::write(stream, req);

            beast::flat_buffer buffer;
            http::response<http::string_body> res;
            http::read(stream, buffer, res);

            std::string answer = parseResponse(res.body());
            callback(answer, "");


            beast::error_code ec;
            stream.shutdown(ec);

        } catch (const std::exception &e) {
            callback("", std::string("Network error: ") + e.what());
        }
    }).detach();
}
