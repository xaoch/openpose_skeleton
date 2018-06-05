#pragma once
#include <iostream>
#include <cstdlib>
#include <string>
#include <cstring>
#include <cctype>
#include <thread>
#include <chrono>
#include "mqtt/async_client.h"
#include "config.h"
#include "audience.h"

//const std::string SERVER_ADDRESS("tcp://localhost:1883");
const std::string SERVER_ADDRESS("tcp://200.126.23.131:1883");
const std::string CLIENT_ID("async_subcribe_cpp");

const std::string PUB_TOPIC("hi");

//const std::vector<std::string> TOPICS = {"rap_start","rap_stop","rap_status"};
const std::vector<std::string> TOPICS = { "rap_stop"};
const char* PAYLOAD1 = "Hello World!";
const auto TIMEOUT = std::chrono::seconds(1);
const int	QOS = 1;
const int	N_RETRY_ATTEMPTS = 5;

/////////////////////////////////////////////////////////////////////////////

// Callbacks for the success or failures of requested actions.
// This could be used to initiate further action, but here we just log the
// results to the console.

class action_listener : public virtual mqtt::iaction_listener
{
	std::string name_;

	void on_failure(const mqtt::token& tok) override {
		std::cout << name_ << " failure";
		if (tok.get_message_id() != 0)
			std::cout << " for token: [" << tok.get_message_id() << "]" << std::endl;
		std::cout << std::endl;
	}

	void on_success(const mqtt::token& tok) override {
		std::cout << name_ << " success";
		if (tok.get_message_id() != 0)
			std::cout << " for token: [" << tok.get_message_id() << "]";
		auto top = tok.get_topics();
		if (top && !top->empty())
			std::cout << "\ttoken topic: '" << (*top)[0] << std::endl;
		
	}

public:
	action_listener(const std::string& name) : name_(name) {}
};

/////////////////////////////////////////////////////////////////////////////

/**
 * Local callback & listener class for use with the client connection.
 * This is primarily intended to receive messages, but it will also monitor
 * the connection to the broker. If the connection is lost, it will attempt
 * to restore the connection and re-subscribe to the topic.
 */
class callback : public virtual mqtt::callback,
					public virtual mqtt::iaction_listener

{
	// Counter for the number of connection retries
	int nretry_;
	// The MQTT client
	mqtt::async_client& cli_;
	// Options to use if we need to reconnect
	mqtt::connect_options& connOpts_;
	// An action listener to display the result of actions.
	action_listener subListener_;

	// This deomonstrates manually reconnecting to the broker by calling
	// connect() again. This is a possibility for an application that keeps
	// a copy of it's original connect_options, or if the app wants to
	// reconnect with different options.
	// Another way this can be done manually, if using the same options, is
	// to just call the async_client::reconnect() method.
	void reconnect() {
		std::this_thread::sleep_for(std::chrono::milliseconds(2500));
		try {
			cli_.connect(connOpts_, nullptr, *this);
		}
		catch (const mqtt::exception& exc) {
			std::cerr << "Error: " << exc.what() << std::endl;
			exit(1);
		}
	}

	// Re-connection failure
	void on_failure(const mqtt::token& tok) override {
		std::cout << "Connection failed" << std::endl;
		if (++nretry_ > N_RETRY_ATTEMPTS)
			exit(1);
		reconnect();
	}

	// Re-connection success
	void on_success(const mqtt::token& tok) override {
		std::cout << "Connection success" << std::endl;
		
		for each (std::string topic in TOPICS)
		{
			std::cout << "Subscribing to topic '" << topic << std::endl;
			cli_.subscribe(topic, QOS, nullptr, subListener_);
		}		
	}

	// Callback for when the connection is lost.
	// This will initiate the attempt to manually reconnect.
	void connection_lost(const std::string& cause) override {
		std::cout << "\nConnection lost" << std::endl;
		if (!cause.empty())
			std::cout << "\tcause: " << cause << std::endl;

		std::cout << "Reconnecting..." << std::endl;
		nretry_ = 0;
		reconnect();
	}

	// Callback for when a message arrives.
	void message_arrived(mqtt::const_message_ptr msg) override {
		std::cout << "Message arrived";
		std::cout << "\ttopic: '" << msg->get_topic() << "'";
		std::cout << "\tpayload: '" << msg->to_string() << std::endl;
		
	/*	if (msg->get_topic() == "rap_start") {
			globals::filename = msg->to_string();
			globals::record = true;			
		}
		else */if (msg->get_topic() == "rap_stop") {
			audience::closeAudience();
			audience::stopRecording();
			//globals::record = false;
		}
		// First use a message pointer.

		//std::cout << "\nSending message..." << std::endl;
		//mqtt::message_ptr pubmsg = mqtt::make_message(PUB_TOPIC, PAYLOAD1);
		//pubmsg->set_qos(QOS);
		//std::cout << "message created" << std::endl;
		//this->cli_.publish(pubmsg)->wait_for(TIMEOUT);		
		//std::cout << "  ...OK" << std::endl;
	}

	void delivery_complete(mqtt::delivery_token_ptr token) override {}

public:																		 
	callback(mqtt::async_client& cli, mqtt::connect_options& connOpts)
				: nretry_(0), cli_(cli), connOpts_(connOpts), subListener_("Subscription") {}
};
