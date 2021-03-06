A library for the Nordic nrf24l01+ module.

This library provides functions for setting up and managing the radio module,
sending and receiving packets, and streaming data between radios.

The rf24 class implements the basic radio access for configuration, status, 
sending and receiving packets.

The RFStream class implements a full Stream class using an rf24 instance.

Example code to setup and use the hardware serial interface:

	rf24 rf(8,9,100,RF24_MAX_SIZE);

	void setup()
	{
	    rf.begin();
	    rf.setRxAddr(1, "addr1");
	    rf.setTxAddr("addr2");
	}

Send a packet:
	
	char data[RF24_MAX_SIZE] = "Hello world";

	rf.send(data, sizeof(data));

Receive a packet:

	char data[RF24_MAX_SIZE];

	if (rf.available()) {
	    rf.read(data, sizeof(data));
	}

Set up a stream:

	rf24 rf(8,9,100,RF24_MAX_SIZE);
	RFStream rfstream;

	void setup()
	{
	    rf.begin();
	    rfstream.begin(&rf);

	    rf.setRxAddr(1, "addr1");
	    rf.setTxAddr("addr2");
	}
	
Send data via a stream:

	rfstream.println("Hello World");
	rfstream.println(42);
	rfstream.println(F("Progmem string"));

Receive data from a stream:

	if (rfstream.available() > 0) {
	    Serial.write(rfstream.read());
	}

Example sketches:
	rfstream1.ino: simple streamer that connects two serial monitors
                       together via two Arduinos and two rf modules.

	rcscan.ino:    "Poor Man's 2.4GHz scanner". Scans the 2.4GHz channels
                       and displays a bar graph of approximate signal strengths.

