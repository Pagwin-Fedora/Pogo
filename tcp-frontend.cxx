#include <boost/asio.hpp>
#include <optional>
#include <numeric>
#include "pogo-daemon-api.h"
using namespace boost::asio;
using ip::tcp;
using std::string;
typedef std::pair<string,std::optional<boost::system::error_code>> read_t;



void Pogo::init_frontend(string config){
			
			boost::asio::io_service io_service;

      tcp::acceptor acceptor(io_service, tcp::endpoint(tcp::v4(),1234));

      tcp::socket socket(io_service);

      acceptor.accept(socket);

      read_t message = recv(socket);
			contactExchange()
}
